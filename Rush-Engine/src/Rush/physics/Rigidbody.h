#ifndef __RIGIDBODY_H__
#define __RIGIDBODY_H__

#include <entt/core/attribute.h>
#include <glm/glm.hpp>

namespace Rush { class PhysicsSystem; }

struct ENTT_API Rigidbody {
private:
	// Primary quntities
	float mass					{ 1.0f };
	float restitution			{ 1.0f };
	float static_friction		{ 0.5f };
	float dynamic_friction		{ 0.3f };
	glm::vec3 velocity			{ 0.0f, 0.0f, 0.0f };
	glm::vec3 angular_velocity	{ 0.0f, 0.0f, 0.0f };
	glm::mat3 inertia_tensor	{ 1.0f };
	
	// Secondary Quantities
	float mass_inv;
	glm::mat3 inertia_inv;

	// Control
	bool getInertiaFromCollider;

	friend class Rush::PhysicsSystem;
public: 

	Rigidbody(float mass = 1.0f, float restitution = 1.0f, float static_friction = 0.5f, float dynamic_friction = 0.3f)
		:mass(mass), restitution(restitution), static_friction(static_friction), dynamic_friction(dynamic_friction), getInertiaFromCollider(true) {
		inertia_tensor = glm::mat3(mass);
		if (mass == 0.0f) {
			mass_inv = 0.0f;
			inertia_inv = glm::mat3(0.0f);
		}
		else {
			mass_inv = 1.0f / mass;
			inertia_inv = glm::inverse(inertia_tensor);
		}
	}

	float GetMass()				{ return mass; }
	float GetInverseMass()		{ return mass_inv; }
	float GetRestitution()		{ return restitution; }
	float GetStaticFriction()	{ return static_friction; }
	float GetDynamicFriction()	{ return dynamic_friction; }
	glm::vec3 GetVelocity()		{ return velocity; }
	glm::vec3 GetAngularVelocity()	{ return angular_velocity; }
	glm::mat3 GetInertiaTensor()	{ return inertia_tensor; }
	glm::mat3 GetInverseInertia()	{ return inertia_inv; }

	void SetMass(float mass) { 
		mass = mass; 
		if (mass == 0.0f) mass_inv = 0.0f;
		else mass_inv = 1.0f / mass;
	}

	void SetInertiaTensor(glm::mat3 tensor) {
		inertia_tensor = tensor;
		inertia_inv = glm::inverse(tensor);
	}

	void SetRestitution(float restitution) { restitution = restitution; }
	void SetStaticFriction(float friction) { static_friction = friction; }
	void SetDynamicFriction(float friction) { dynamic_friction = friction; }
	void ResetInertia() { 
		getInertiaFromCollider = true;
		inertia_tensor = glm::mat3(mass);
		inertia_inv = glm::inverse(inertia_tensor);
	}
};

#endif //__RIGIDBODY_H__