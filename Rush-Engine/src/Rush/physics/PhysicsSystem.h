#ifndef __PHYSICS_SYSTEM_H__
#define __PHYSICS_SYSTEM_H__

#include "Rush/core/core.h"
#include "Rush/scene/Scene.h"
#include "CollisionDetection.h"

namespace Rush {

class RUSH_API PhysicsSystem {
private:
	PhysicsSystem() = delete;
	~PhysicsSystem() = delete;

	static float s_Gravity;
	static int s_SolverIterations;
	static std::vector<Collision> s_Collisions;
public:
	static void StepPhysics(Scene& scene);
	static void DrawDebug(Scene& scene);

	static void SetGravity(float acc) { s_Gravity = acc; }
	static float GetGravity() { return s_Gravity; }
};

};

#endif //__PHYSICS_SYSTEM_H__