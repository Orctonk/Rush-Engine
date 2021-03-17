#ifndef __AABB_COLLIDER_H__
#define __AABB_COLLIDER_H__

#include "Rush/core/Core.h"
#include "BaseCollider.h"
#include <glm/glm.hpp>

#include <entt/core/attribute.h>

struct ENTT_API BoxCollider : public BaseCollider {
	glm::vec3 min	{ -1.0f, -1.0f, -1.0f };
	glm::vec3 max	{  1.0f,  1.0f,  1.0f };

	BoxCollider() : BaseCollider(ColliderType::BoxCollider) {}
	BoxCollider(BoxCollider&& other) : BaseCollider(ColliderType::BoxCollider) { min = other.min; max = other.max; }
	BoxCollider& operator=(const BoxCollider& other) { min = other.min; max = other.max; return *this; }

	virtual bool CheckCollision(const BaseCollider& other, const TransformComponent& t1, const TransformComponent& t2, Manifold* manifold) override;
};

#endif // __AABB_COLLIDER_H__