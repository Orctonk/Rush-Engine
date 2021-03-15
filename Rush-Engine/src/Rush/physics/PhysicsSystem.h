#ifndef __PHYSICS_SYSTEM_H__
#define __PHYSICS_SYSTEM_H__

#include "Rush/core/core.h"
#include "Rush/scene/Scene.h"

namespace Rush {

class RUSH_API PhysicsSystem {
private:
	PhysicsSystem() = delete;
	~PhysicsSystem() = delete;

	static float s_Gravity;
public:
	static void StepPhysics(Scene& scene);

	static void SetGravity(float acc) { s_Gravity = acc; }
	static float GetGravity() { return s_Gravity; }
};

};

#endif //__PHYSICS_SYSTEM_H__