#ifndef __AABB_COLLIDER_H__
#define __AABB_COLLIDER_H__

#include "Rush/core/Core.h"
#include "BaseCollider.h"
#include <glm/glm.hpp>

#include <entt/core/attribute.h>

struct ENTT_API AABBCollider : public BaseCollider {
	glm::vec3 min	{ -1.0f, -1.0f, -1.0f };
	glm::vec3 max	{  1.0f,  1.0f,  1.0f };

	AABBCollider() : BaseCollider(ColliderType::AABBCollider) {}
	AABBCollider(AABBCollider&& other) : BaseCollider(ColliderType::AABBCollider) { min = other.min; max = other.max; }
	AABBCollider& operator=(const AABBCollider& other) { min = other.min; max = other.max; return *this; }

	virtual bool CheckCollision(const BaseCollider& other, const TransformComponent& t1, const TransformComponent& t2, Manifold* manifold) override;
};

#endif // __AABB_COLLIDER_H__