#include "PhysicsSystem.h"

#include "Rigidbody.h"
#include "CollisionDetection.h"
#include "Rush/core/Time.h"
#include "Rush/scene/components/TransformComponent.h"

namespace Rush {

float PhysicsSystem::s_Gravity = 0.0f;

void PhysicsSystem::StepPhysics(Scene &scene) {
	auto* reg = scene.GetRegistry();
	auto rbgroup = reg->group<Rigidbody>(entt::get<TransformComponent>);
	for (auto e : rbgroup) {
		RUSH_ASSERT(reg->all_of<TransformComponent>(e));
		auto& rb = rbgroup.get<Rigidbody>(e);
		if (rb.mass_inv != 0.0f) {
			auto& t = rbgroup.get<TransformComponent>(e);
			t.Translate(rb.velocity * (float)Time::FixedDelta());
			rb.velocity += glm::vec3{ 0.0f,- s_Gravity * Time::FixedDelta(),0.0f };
		}
	}
	std::vector<Collision> collisions = CollisionDetection::DetectCollisions(scene);
	for (auto c : collisions) {
		if (c.e1.HasComponent<Rigidbody>() && c.e2.HasComponent<Rigidbody>()) {
			auto &rb1 = c.e1.GetComponent<Rigidbody>();
			auto &rb2 = c.e2.GetComponent<Rigidbody>();
			auto& t1 = c.e1.GetComponent<TransformComponent>();
			auto& t2 = c.e2.GetComponent<TransformComponent>();
			glm::vec3 n = c.manifold.collisionNormal;
			float pd = c.manifold.penetrationDepth;
			float avgRest = (rb1.restitution + rb2.restitution) / 2.0f + 1.0f;
			float impulseMag = avgRest * (glm::dot(rb2.velocity, n) - glm::dot(rb1.velocity, n)) * (rb1.mass_inv + rb2.mass_inv);
			glm::vec3 impulse = n * impulseMag;
			rb1.velocity += impulse * rb1.mass_inv;
			rb2.velocity += impulse * -rb2.mass_inv;

			glm::vec3 correction = n * (pd * 0.2f / (rb1.mass_inv + rb2.mass_inv));
			t1.Translate(correction * -rb1.mass_inv);
			t2.Translate(correction * rb2.mass_inv);
		}
	}
}

};