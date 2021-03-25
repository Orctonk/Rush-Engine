#ifndef __SPHERE_COLLIDER_H__
#define __SPHERE_COLLIDER_H__

#include "Rush/core/Core.h"
#include "BaseCollider.h"
#include <glm/glm.hpp>

#include <entt/core/attribute.h>

struct ENTT_API SphereCollider : public BaseCollider {
	float radius		{ 1.0f };
	glm::vec3 offset	{ 0.0f,0.0f,0.0f };

	SphereCollider() : BaseCollider(ColliderType::SphereCollider) {}
	SphereCollider(SphereCollider&& other) : BaseCollider(ColliderType::SphereCollider) { radius = other.radius; offset = other.offset; }
	SphereCollider& operator=(const SphereCollider& other) { radius = other.radius; offset = other.offset; return *this; }
};

#endif // __SPHERE_COLLIDER_H__