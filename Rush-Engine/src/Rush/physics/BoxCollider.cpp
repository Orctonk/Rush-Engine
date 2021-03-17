#include "BoxCollider.h"

#include "SphereCollider.h"
#include "Rush/core/Logger.h"

#include <glm/gtx/norm.hpp>

bool BoxCollider::CheckCollision(const BaseCollider& other, const TransformComponent& t1, const TransformComponent& t2, Manifold* manifold) {
	if (other.type == ColliderType::SphereCollider) { // AABB vs Sphere
		SphereCollider& o = (SphereCollider&)other;
		glm::vec3 max = max + t1.GetTranslation();
		glm::vec3 min = min + t1.GetTranslation();
		glm::vec3 c1 = (max + min) / 2.0f;
		glm::vec3 c2 = t2.GetRotation() * glm::vec4(o.offset, 0.0f);
		c2 += t2.GetTranslation();

		glm::vec3 c1toc2 = c2 - c1;
		glm::vec3 ex = (max - min) / 2.0f;
		glm::vec3 clamped = glm::clamp(c1toc2, -ex, ex);

		bool inside = false;

		if (c1toc2 == clamped) {
			inside = true;

			if (glm::abs(clamped.x) > glm::abs(clamped.y) && glm::abs(clamped.x) > glm::abs(clamped.z))
				clamped.x = glm::sign(clamped.x) * ex.x;
			else if (glm::abs(clamped.y) > glm::abs(clamped.z))
				clamped.y = glm::sign(clamped.y) * ex.y;
			else
				clamped.z = glm::sign(clamped.z) * ex.z;
		}

		glm::vec3 normal = c1toc2 - clamped;
		float sqrDist = glm::length2(normal);

		if (sqrDist > o.radius * o.radius && !inside)
			return false;

		float dist = glm::sqrt(sqrDist);

		if (inside) 
			manifold->collisionNormal = -glm::normalize(normal);
		else
			manifold->collisionNormal = glm::normalize(normal);
		manifold->penetrationDepth = o.radius - dist;
		manifold->pointOfImpact = c1 + manifold->collisionNormal * (o.radius - manifold->penetrationDepth / 2.0f);

		return true;
	}
	else if (other.type == ColliderType::BoxCollider) { // AABB vs AABB
		BoxCollider& o = (BoxCollider&)other;
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
			
		glm::vec3 signvec = glm::sign(c1toc2);
		glm::vec3 borderpos = c1 + signvec * ex1;
		manifold->pointOfImpact = borderpos - signvec * overlap;

		return true;
	}
	else {
		RUSH_LOG_ERROR("Unexpected collider type");
		RUSH_ASSERT(false);
	}
}