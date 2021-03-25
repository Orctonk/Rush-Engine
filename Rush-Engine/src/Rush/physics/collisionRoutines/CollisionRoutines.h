#ifndef __COLLISION_ROUTINES_H__
#define __COLLISION_ROUTINES_H__

#include "../BaseCollider.h"

typedef bool (*CollisionRoutine)(const BaseCollider&, const BaseCollider&, const TransformComponent&, const TransformComponent&, Manifold*);

bool SphereSphereCollision(const BaseCollider&, const BaseCollider&, const TransformComponent&, const TransformComponent&, Manifold*);
bool SphereBoxCollision(const BaseCollider&, const BaseCollider&, const TransformComponent&, const TransformComponent&, Manifold*);

bool BoxSphereCollision(const BaseCollider&, const BaseCollider&, const TransformComponent&, const TransformComponent&, Manifold*);
bool BoxBoxCollision(const BaseCollider&, const BaseCollider&, const TransformComponent&, const TransformComponent&, Manifold*);

#endif // __COLLISION_ROUTINES_H__