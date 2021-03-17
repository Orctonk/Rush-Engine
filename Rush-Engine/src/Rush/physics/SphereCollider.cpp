#include "SphereCollider.h"

#include "Rush/core/Logger.h"
#include "BoxCollider.h"

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
		manifold->pointOfImpact = c1 + (radius - manifold->penetrationDepth);
		return false;
	}
	else if (other.type == ColliderType::BoxCollider) { // AABB vs Sphere
		BoxCollider& o = (BoxCollider&)other;
		glm::vec3 max = o.max + t2.GetTranslation();
		glm::vec3 min = o.min + t2.GetTranslation();
		glm::vec3 c2 = (max + min) / 2.0f;
		glm::vec3 c1 = t1.GetRotation() * glm::vec4(offset, 0.0f);
		c1 += t1.GetTranslation();

		glm::vec3 c2toc1 = c1 - c2;
		glm::vec3 ex = (max - min) / 2.0f;
		glm::vec3 clamped = glm::clamp(c2toc1, -ex, ex);

		bool inside = false;

		if (c2toc1 == clamped) {
			inside = true;

			if (glm::abs(clamped.x) > glm::abs(clamped.y) && glm::abs(clamped.x) > glm::abs(clamped.z))
				clamped.x = glm::sign(clamped.x) * ex.x;
			else if (glm::abs(clamped.y) > glm::abs(clamped.z))
				clamped.y = glm::sign(clamped.y) * ex.y;
			else
				clamped.z = glm::sign(clamped.z) * ex.z;
		}

		glm::vec3 normal = clamped - c2toc1;
		float sqrDist = glm::length2(normal);

		if (sqrDist > radius * radius && !inside)
			return false;

		float dist = glm::sqrt(sqrDist);

		if (inside)
			manifold->collisionNormal = -glm::normalize(normal);
		else
			manifold->collisionNormal = glm::normalize(normal);
		manifold->penetrationDepth = radius - dist;
		manifold->pointOfImpact = c1 + manifold->collisionNormal * (radius - manifold->penetrationDepth / 2.0f);

		return true;
	}
	else {
		RUSH_LOG_ERROR("Unexpected collider type");
		RUSH_ASSERT(false);
	}
}