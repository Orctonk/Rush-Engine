#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include <entt/core/attribute.h>
#include <glm/glm.hpp>

struct ENTT_API Rigidbody {
	float mass_inv			{ 1.0f };
	float restitution		{ 1.0f };
	float static_friction	{ 0.5f };
	float dynamic_friction	{ 0.3f };
	glm::vec3 velocity		{ 0.0f, 0.0f, 0.0f };
	glm::mat3 inertia_tensor{ 1.0f };
};

#endif //__RIGIDBODY_H__