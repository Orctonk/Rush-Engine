#ifndef __COLLISION_DETECTION_H__
#define __COLLISION_DETECTION_H__

#include <vector>
#include <optional>

#include "Rush/core/core.h"
#include "Rush/scene/Scene.h"
#include "Rush/scene/Entity.h"
#include "SphereCollider.h"

namespace Rush {

struct RUSH_API Collision {
	Entity e1, e2;
	Manifold manifold;
};

class RUSH_API CollisionDetection {
private:
	CollisionDetection() = delete;
	~CollisionDetection() = delete;

	static bool CheckCollision(SphereCollider& s1, SphereCollider& s2);
public:

	static std::vector<Collision> DetectCollisions(Scene &scene);
};

};

#endif // !__COLLISION_DETECTION_H__
