#ifndef __BASE_COLLIDER_H__
#define __BASE_COLLIDER_H__

#include "Rush/core/Core.h"

#include "Rush/scene/components/TransformComponent.h"

struct RUSH_API Manifold {
	glm::vec3 collisionNormal;
	float penetrationDepth;
};

enum class RUSH_API ColliderType {
	AABBCollider,
	SphereCollider
};

struct RUSH_API BaseCollider {
	const ColliderType type;

	BaseCollider(ColliderType type) : type(type) {};

	virtual bool CheckCollision(const BaseCollider& other, const TransformComponent &t1, const TransformComponent &t2, Manifold *manifold) = 0;
};

#endif // !__BASE_COLLIDER_H__
