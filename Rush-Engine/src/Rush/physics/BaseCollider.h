#ifndef __BASE_COLLIDER_H__
#define __BASE_COLLIDER_H__

#include "Rush/core/Core.h"

#include "Rush/scene/components/TransformComponent.h"

struct RUSH_API Contact {
	glm::vec3 position;
	float depth;
	float total_impulse = 0.0f;
	float total_friction = 0.0f;
};

// Data about collision, always relative to the invoking collision shape
struct RUSH_API Manifold {
	glm::vec3 collisionNormal;
	float penetrationDepth;
	int numContacts;
	Contact contactPoints[4];
};

enum class RUSH_API ColliderType {
	SphereCollider = 0,
	BoxCollider,
	Count
};

struct RUSH_API BaseCollider {
	const ColliderType type;

	BaseCollider(ColliderType type) : type(type) {};

	bool CheckCollision(const BaseCollider& other, const TransformComponent &t1, const TransformComponent &t2, Manifold *manifold);
};

#endif // !__BASE_COLLIDER_H__
