#include "AABBCollider.h"

#include "SphereCollider.h"
#include "Rush/core/Logger.h"

#include <glm/gtx/norm.hpp>

bool AABBCollider::CheckCollision(const BaseCollider& other, const TransformComponent& t1, const TransformComponent& t2, Manifold* manifold) {
	if (other.type == ColliderType::SphereCollider) { // AABB vs Sphere
		SphereCollider& o = (SphereCollider&)other;
		glm::vec3 max = max + t1.GetTranslation();
		glm::vec3 min = min + t1.GetTranslation();
		glm::vec3 c2 = t2.GetRotation() * glm::vec4(o.offset, 0.0f);
		c2 += t2.GetTranslation();
		// Collision code
		return false;
	}
	else if (other.type == ColliderType::AABBCollider) { // AABB vs AABB
		AABBCollider& o = (AABBCollider&)other;
		glm::vec3 max1 = max + t1.GetTranslation();
		glm::vec3 min1 = min + t1.GetTranslation();
		glm::vec3 max2 = o.max + t2.GetTranslation();
		glm::vec3 min2 = o.min + t2.GetTranslation();

		glm::vec3 c1 = (max1 + min1) / 2.0f;
		glm::vec3 c2 = (max2 + min2) / 2.0f;
		glm::vec3 c1toc2 = c2 - c1;
		glm::vec3 ex1 = (max1 - min1) / 2.0f;
		glm::vec3 ex2 = (max2 - min2) / 2.0f;
		
		glm::vec3 overlap = ex1 + ex2 - glm::abs(c1toc2);
		if (overlap.x <= 0.0f || overlap.y <= 0.0f || overlap.z <= 0.0f)
			return false;
		if (overlap.x < overlap.y && overlap.x < overlap.z) {
			if (c1toc2.x > 0.0f)	manifold->collisionNormal = glm::vec3( 1.0f, 0.0f, 0.0f);
			else					manifold->collisionNormal = glm::vec3(-1.0f, 0.0f, 0.0f);
			manifold->penetrationDepth = overlap.x;
		} else if (overlap.y < overlap.z) {
			if (c1toc2.y > 0.0f)	manifold->collisionNormal = glm::vec3( 0.0f, 1.0f, 0.0f);
			else					manifold->collisionNormal = glm::vec3( 0.0f,-1.0f, 0.0f);
			manifold->penetrationDepth = overlap.y;
		} else {
			if (c1toc2.z > 0.0f)	manifold->collisionNormal = glm::vec3( 0.0f, 0.0f, 1.0f);
			else					manifold->collisionNormal = glm::vec3( 0.0f, 0.0f,-1.0f);
			manifold->penetrationDepth = overlap.z;
		}
		return true;
	}
	else {
		RUSH_LOG_ERROR("Unexpected collider type");
		RUSH_ASSERT(false);
	}
}