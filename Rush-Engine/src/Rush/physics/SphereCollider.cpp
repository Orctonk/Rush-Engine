#include "SphereCollider.h"

#include "Rush/core/Logger.h"
#include "AABBCollider.h"

#include <glm/gtx/norm.hpp>

bool SphereCollider::CheckCollision(const BaseCollider& other, const TransformComponent& t1, const TransformComponent& t2, Manifold* manifold) {
	if (other.type == ColliderType::SphereCollider) { // Sphere VS Sphere
		SphereCollider& o = (SphereCollider &)other;
		glm::vec3 c1 = t1.GetRotation() * glm::vec4(offset,0.0f);
		glm::vec3 c2 = t2.GetRotation() * glm::vec4(o.offset,0.0f);
		c1 += t1.GetTranslation();
		c2 += t2.GetTranslation();
		float totalRadius = radius + o.radius;
		float d2 = glm::distance2(c1, c2);
		if (d2 < totalRadius * totalRadius) {
			manifold->collisionNormal = glm::normalize(c2 - c1);
			manifold->penetrationDepth = totalRadius - glm::sqrt(d2);
			return true;
		}
		return false;
	}
	else if (other.type == ColliderType::AABBCollider) { // AABB vs Sphere
		AABBCollider& o = (AABBCollider&)other;
		glm::vec3 c1 = t1.GetRotation() * glm::vec4(offset, 0.0f);
		c1 += t1.GetTranslation();
		glm::vec3 max2 = o.max + t2.GetTranslation();
		glm::vec3 min2 = o.min + t2.GetTranslation();
		// Collision code
		return false;
	}
	else {
		RUSH_LOG_ERROR("Unexpected collider type");
		RUSH_ASSERT(false);
	}
}