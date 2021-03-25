#ifndef __AABB_COLLIDER_H__
#define __AABB_COLLIDER_H__

#include "Rush/core/Core.h"
#include "BaseCollider.h"
#include <glm/glm.hpp>

#include <entt/core/attribute.h>

struct ENTT_API BoxCollider : public BaseCollider {
	glm::vec3 center{ 0.0f, 0.0f, 0.0f };
	glm::vec3 size	{ 1.0f, 1.0f, 1.0f };

	BoxCollider() : BaseCollider(ColliderType::BoxCollider) {}
	BoxCollider(BoxCollider&& other) : BaseCollider(ColliderType::BoxCollider) { center = other.center; size = other.size; }
	BoxCollider& operator=(const BoxCollider& other) { center = other.center; size = other.size; return *this; }
};

#endif // __AABB_COLLIDER_H__