#include "PhysicsSystem.h"

#include "Rigidbody.h"
#include "Rush/core/Time.h"
#include "Rush/graphics/LineRenderer.h"
#include "Rush/scene/components/TransformComponent.h"

namespace Rush {

float PhysicsSystem::s_Gravity = 0.0f;
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
			rb.velocity += gravitydV;
		}
	}

	s_Collisions = CollisionDetection::DetectCollisions(scene);
	for (auto &c : s_Collisions) {
		LineRenderer::DrawLine(c.manifold.pointOfImpact, c.manifold.pointOfImpact + c.manifold.collisionNormal, glm::vec4(1.0f));
		if (c.e1.HasComponent<Rigidbody>() && c.e2.HasComponent<Rigidbody>()) {
			auto &rb1 = c.e1.GetComponent<Rigidbody>();
			auto &rb2 = c.e2.GetComponent<Rigidbody>();
			if (rb1.mass_inv + rb2.mass_inv == 0.0f)
				continue;
			auto& t1 = c.e1.GetComponent<TransformComponent>();
			auto& t2 = c.e2.GetComponent<TransformComponent>();
			glm::vec3 n = c.manifold.collisionNormal;
			if (glm::dot(rb2.velocity - rb1.velocity, n) > 0.0f)
				continue;
			float pd = c.manifold.penetrationDepth;
			// TODO: Add method to change the combination of restitutions
			float avgRest = (rb1.restitution + rb2.restitution) / 2.0f + 1.0f;
			float impulseMag = avgRest * (glm::dot(rb2.velocity, n) - glm::dot(rb1.velocity, n)) / (rb1.mass_inv + rb2.mass_inv);
			glm::vec3 impulse = n * impulseMag;
			glm::vec3 gravAlongN = glm::dot(gravitydV, n) * n;	// Correct for gravity induced this step to allow objects to come to rest
			rb1.velocity += (impulse + gravAlongN) * rb1.mass_inv;
			rb2.velocity += (impulse + gravAlongN) * -rb2.mass_inv;

			glm::vec3 relativeVelocity = rb2.velocity - rb1.velocity;
			glm::vec3 tangent = relativeVelocity - glm::dot(relativeVelocity, n) * n;
			if (glm::any(glm::epsilonNotEqual(tangent, glm::vec3(0.0f), 0.02f)))
				tangent = glm::normalize(tangent);
			float frictionMag = glm::dot(relativeVelocity, tangent) / (rb1.mass_inv + rb2.mass_inv);

			// TODO: Add method to change the combination of friction
			float staticFric = (rb1.static_friction + rb2.static_friction) / 2.0f;
			glm::vec3 frictionImpulse;
			if (glm::abs(frictionMag) < glm::abs(impulseMag * staticFric))
				frictionImpulse = frictionMag * tangent;
			else {
				float dynamicFric = (rb1.dynamic_friction + rb2.dynamic_friction) / 2.0f;
				frictionImpulse = -impulseMag * dynamicFric * tangent ;
			}

			rb1.velocity += frictionImpulse * rb1.mass_inv;
			rb2.velocity += frictionImpulse * -rb2.mass_inv;

			glm::vec3 correction = n * (glm::max(pd-0.01f,0.0f) * 0.2f / (rb1.mass_inv + rb2.mass_inv));
			t1.Translate(correction * -rb1.mass_inv);
			t2.Translate(correction * rb2.mass_inv);
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
	for (const auto &c : s_Collisions)
		LineRenderer::DrawLine(c.manifold.pointOfImpact, c.manifold.pointOfImpact + c.manifold.collisionNormal, glm::vec4(1.0f));

	LineRenderer::EndScene();
}

};