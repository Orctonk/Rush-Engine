#include "PhysicsSystem.h"

#include "Rigidbody.h"
#include "Rush/core/Time.h"
#include "Rush/graphics/LineRenderer.h"
#include "Rush/scene/components/TransformComponent.h"

namespace Rush {

float PhysicsSystem::s_Gravity = 0.0f;
int PhysicsSystem::s_SolverIterations = 4;
std::vector<Collision> PhysicsSystem::s_Collisions;

void PhysicsSystem::StepPhysics(Scene &scene) {
	auto* reg = scene.GetRegistry();
	auto rbgroup = reg->group<Rigidbody>(entt::get<TransformComponent>);
	glm::vec3 gravitydV = glm::vec3{ 0.0f,-s_Gravity * Time::FixedDelta(),0.0f };
	for (auto e : rbgroup) {
		RUSH_ASSERT(reg->all_of<TransformComponent>(e));
		auto& rb = rbgroup.get<Rigidbody>(e);
		if (rb.mass_inv != 0.0f) {
			auto& t = rbgroup.get<TransformComponent>(e);
			t.Translate(rb.velocity * (float)Time::FixedDelta());
			glm::quat q0 = t.GetRotation();
			glm::vec3 av = rb.angular_velocity * (float)Time::FixedDelta();
			float avNorm = glm::length(av);
			if(avNorm != 0)
				t.SetRotation(glm::quat(glm::cos(avNorm/2.0f),av/avNorm * sin(avNorm/2.0f)) * q0);
			rb.velocity += gravitydV;
		}
	}

	s_Collisions = CollisionDetection::DetectCollisions(scene);
	for (int i = 0; i < s_SolverIterations; i++) {
		for (auto &c : s_Collisions) {
			if (c.e1.HasComponent<Rigidbody>() && c.e2.HasComponent<Rigidbody>()) {
				auto &rb1 = c.e1.GetComponent<Rigidbody>();
				auto &rb2 = c.e2.GetComponent<Rigidbody>();
				if (rb1.mass_inv + rb2.mass_inv == 0.0f)
					continue;
				auto& t1 = c.e1.GetComponent<TransformComponent>();
				auto& t2 = c.e2.GetComponent<TransformComponent>();

				// TODO: Add method to change the combination of restitutions
				float avgRest = (rb1.restitution + rb2.restitution) / 2.0f + 1.0f;
				// TODO: Add method to change the combination of friction
				float staticFric = (rb1.static_friction + rb2.static_friction) / 2.0f;
				float dynamicFric = (rb1.dynamic_friction + rb2.dynamic_friction) / 2.0f;

				for (int j = 0; j < c.manifold.numContacts; j++) {
					glm::vec3 n = c.manifold.collisionNormal;
					glm::vec3 colRelC1 = c.manifold.contactPoints[j].position - t1.GetTranslation();
					glm::vec3 colRelC2 = c.manifold.contactPoints[j].position - t2.GetTranslation();
					glm::vec3 vel1 = rb1.velocity + glm::cross(rb1.angular_velocity, colRelC1);
					glm::vec3 vel2 = rb2.velocity + glm::cross(rb2.angular_velocity, colRelC2);
					glm::vec3 relativeVelocity = vel2 - vel1;

					if (glm::dot(relativeVelocity, n) > 0.0f)
						continue;
					float pd = c.manifold.penetrationDepth;

					glm::vec3 c1Cross = rb1.inertia_inv * glm::cross(colRelC1, c.manifold.collisionNormal);
					glm::vec3 c2Cross = rb2.inertia_inv * glm::cross(colRelC2, c.manifold.collisionNormal);
					
					float impulseMag = avgRest * (glm::dot(relativeVelocity, n)) / 
						(rb1.mass_inv + rb2.mass_inv + glm::dot(glm::cross(c1Cross,colRelC1) + glm::cross(c2Cross,colRelC2),n));
					glm::vec3 impulse = n * impulseMag;
					glm::vec3 gravAlongN = glm::dot(gravitydV, n) * n;	// Correct for gravity induced this step to allow objects to come to rest
					rb1.velocity += (impulse + gravAlongN) * rb1.mass_inv;
					rb1.angular_velocity += impulseMag * c1Cross;
					rb2.velocity += (impulse + gravAlongN) * -rb2.mass_inv;
					rb2.angular_velocity -= impulseMag * c2Cross;

					vel1 = rb1.velocity + glm::cross(rb1.angular_velocity, colRelC1);
					vel2 = rb2.velocity + glm::cross(rb2.angular_velocity, colRelC2);
					relativeVelocity = vel2 - vel1;

					glm::vec3 tangent = relativeVelocity - glm::dot(relativeVelocity, n) * n;
					if (glm::any(glm::epsilonNotEqual(tangent, glm::vec3(0.0f), 0.02f)))
						tangent = glm::normalize(tangent);

					c1Cross = rb1.inertia_inv * glm::cross(colRelC1, tangent);
					c2Cross = rb2.inertia_inv * glm::cross(colRelC2, tangent);

					float frictionMag = glm::dot(relativeVelocity, tangent) / 
						(rb1.mass_inv + rb2.mass_inv + glm::dot(glm::cross(c1Cross,colRelC1)+glm::cross(c2Cross,colRelC2),tangent));

					glm::vec3 frictionImpulse;
					if (glm::abs(frictionMag) < glm::abs(impulseMag * staticFric))
						frictionImpulse = frictionMag * tangent;
					else {
						frictionMag = -impulseMag * dynamicFric;
						frictionImpulse = frictionMag * tangent;
					}

					rb1.velocity += frictionImpulse * rb1.mass_inv;
					rb1.angular_velocity += rb1.inertia_inv * glm::cross(colRelC1,frictionImpulse);
					rb2.velocity += frictionImpulse * -rb2.mass_inv;
					rb2.angular_velocity -= rb2.inertia_inv * glm::cross(colRelC2, frictionImpulse);

					glm::vec3 correction = n * (glm::max(pd-0.01f,0.0f) * 0.2f / (rb1.mass_inv + rb2.mass_inv));
					t1.Translate(correction * -rb1.mass_inv);
					t2.Translate(correction * rb2.mass_inv);
				}
			}
		}
	}
}

void PhysicsSystem::DrawDebug(Scene& scene) {
	auto* reg = scene.GetRegistry();
	LineRenderer::GetAPI()->SetOption(DepthTest::None);
	auto camGroup = reg->view<CameraComponent, TransformComponent>();
	for (auto e : camGroup) {
		auto cc = camGroup.get<CameraComponent>(e);
		if (cc.main) {
			auto tc = camGroup.get<TransformComponent>(e);
			glm::mat4 toFront(1.0f);
			toFront[2][2] = 0.5f;
			glm::mat4 model = tc.GetModelMatrix();
			LineRenderer::BeginScene(toFront * cc.camera.GetProjection(), glm::inverse(model));
		}
	}
	auto transGroup = reg->view<TransformComponent,Rigidbody>();
	for (auto e : transGroup) {
		auto& t = transGroup.get<TransformComponent>(e);
		glm::vec3 pos = t.GetTranslation();
		LineRenderer::DrawLine(pos, pos + t.GetRight(), glm::vec4(1.0f,0.0f,0.0f,1.0f));
		LineRenderer::DrawLine(pos, pos + t.GetUp(), glm::vec4(0.0f,1.0f,0.0f,1.0f));
		LineRenderer::DrawLine(pos, pos + t.GetBack(), glm::vec4(0.0f,0.0f,1.0f,1.0f));
	}
	for (const auto& c : s_Collisions) {
		for (int i = 0; i < c.manifold.numContacts; i++) {
			LineRenderer::DrawLine(c.manifold.contactPoints[i].position, c.manifold.contactPoints[i].position + c.manifold.collisionNormal, glm::vec4(1.0f));
		}
	}

	LineRenderer::EndScene();
}

};