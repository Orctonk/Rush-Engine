#include "BaseCollider.h"

#include "collisionRoutines/CollisionRoutines.h"

CollisionRoutine CollisionJumpTable[(int)ColliderType::Count][(int)ColliderType::Count] = {
	{	SphereSphereCollision,	SphereBoxCollision	},
	{	BoxSphereCollision,		BoxBoxCollision		}
};

bool BaseCollider::CheckCollision(const BaseCollider& other, const TransformComponent& t1, const TransformComponent& t2, Manifold* manifold) {
	return CollisionJumpTable[(int)type][(int)other.type](*this, other, t1, t2, manifold);
}