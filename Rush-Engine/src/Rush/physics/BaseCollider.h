#ifndef __BASE_COLLIDER_H__
#define __BASE_COLLIDER_H__

#include "Rush/core/Core.h"

#include "Rush/scene/components/TransformComponent.h"

// TODO: Currently each collider type requires collision checks to be implemented for every other class. 
// This means that there is n^2 collision functions and most are duplicated. 
// Example: Sphere checks Sphere vs AABB and AABB checks AABB vs Sphere even though these are cleary the same function.

// Data about collision, always relative to the invoking collision shape
struct RUSH_API Manifold {
	glm::vec3 collisionNormal;
	glm::vec3 pointOfImpact;
	float penetrationDepth;
};

enum class RUSH_API ColliderType {
	BoxCollider,
	SphereCollider
};

struct RUSH_API BaseCollider {
	const ColliderType type;

	BaseCollider(ColliderType type) : type(type) {};

	virtual bool CheckCollision(const BaseCollider& other, const TransformComponent &t1, const TransformComponent &t2, Manifold *manifold) = 0;
};

#endif // !__BASE_COLLIDER_H__
