#include "CollisionRoutines.h"

#include "../SphereCollider.h"
#include "../BoxCollider.h"

#include <glm/gtx/norm.hpp>

bool SphereSphereCollision(const BaseCollider& c1, const BaseCollider& c2, const TransformComponent& t1, const TransformComponent& t2, Manifold* m) {
	SphereCollider& s1 = (SphereCollider&)c1;
	SphereCollider& s2 = (SphereCollider&)c2;
	glm::vec3 p1 = t1.GetRotation() * glm::vec4(s1.offset, 0.0f);
	glm::vec3 p2 = t2.GetRotation() * glm::vec4(s2.offset, 0.0f);
	p1 += t1.GetTranslation();
	p2 += t2.GetTranslation();
	float totalRadius = s1.radius + s2.radius;
	float d2 = glm::distance2(p1, p2);
	if (d2 < totalRadius * totalRadius) {
		m->collisionNormal = glm::normalize(p2 - p1);
		m->penetrationDepth = totalRadius - glm::sqrt(d2);
		m->numContacts = 1;
		m->contactPoints[0].depth = m->penetrationDepth;
		m->contactPoints[0].position = p1 + m->collisionNormal * (s1.radius - m->penetrationDepth);
		return true;
	}
	m->numContacts = 0;
	return false;
}

bool SphereBoxCollision(const BaseCollider& c1, const BaseCollider& c2, const TransformComponent& t1, const TransformComponent& t2, Manifold* m) {
	m->numContacts = 0;
	SphereCollider& s1 = (SphereCollider&)c1;
	BoxCollider& b2 = (BoxCollider&)c2;
	glm::vec3 p1 = t1.GetRotation() * glm::vec4(s1.offset, 0.0f);
	p1 += t1.GetTranslation();
	glm::vec3 p2 = b2.center + t2.GetTranslation();

	glm::vec3 pr = p1 - p2;
	glm::vec3 ex = b2.size / 2.0f;
	glm::vec3 clamped = glm::clamp(pr, -ex, ex);

	bool inside = false;

	if (pr == clamped) {
		inside = true;

		if (glm::abs(clamped.x) > glm::abs(clamped.y) && glm::abs(clamped.x) > glm::abs(clamped.z))
			clamped.x = glm::sign(clamped.x) * ex.x;
		else if (glm::abs(clamped.y) > glm::abs(clamped.z))
			clamped.y = glm::sign(clamped.y) * ex.y;
		else
			clamped.z = glm::sign(clamped.z) * ex.z;
	}

	glm::vec3 normal = clamped - pr;
	float sqrDist = glm::length2(normal);

	if (sqrDist > s1.radius * s1.radius && !inside)
		return false;

	float dist = glm::sqrt(sqrDist);

	if (inside)
		m->collisionNormal = -glm::normalize(normal);
	else
		m->collisionNormal = glm::normalize(normal);
	m->penetrationDepth = s1.radius - dist;
	m->numContacts = 1;
	m->contactPoints[0].depth = m->penetrationDepth;
	m->contactPoints[0].position = p1 + m->collisionNormal * (s1.radius - m->penetrationDepth / 2.0f);

	return true;
}