#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include <entt/core/attribute.h>
#include <glm/glm.hpp>

struct ENTT_API Rigidbody {
	float mass_inv		{ 1.0f };
	float restitution	{ 1.0f };
	glm::vec3 velocity	{ 0.0f, 0.0f, 0.0f };
};

#endif //__RIGIDBODY_H__