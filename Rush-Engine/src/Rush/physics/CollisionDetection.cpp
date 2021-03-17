#include "CollisionDetection.h"

#include "Rush/scene/components/TransformComponent.h"

namespace Rush {

std::vector<Collision> CollisionDetection::DetectCollisions(Scene& scene) {
	std::vector<Collision> detectedCollisions;
	std::vector<std::pair<entt::entity,BaseCollider*>> colliders;
	auto *reg = scene.GetRegistry();
	auto sphereGroup = reg->view<SphereCollider>();
	for (auto it = sphereGroup.begin(); it != sphereGroup.end(); it++) {
		colliders.push_back({ *it,&sphereGroup.get<SphereCollider>(*it) });
	}
	auto aabbGroup = reg->view<BoxCollider>();
	for (auto it = aabbGroup.begin(); it != aabbGroup.end(); it++) {
		colliders.push_back({ *it,&aabbGroup.get<BoxCollider>(*it) });
	}

	Manifold m;
	for (auto it1 = colliders.begin(); it1 < colliders.end(); it1++) {
		auto &t1 = reg->get<TransformComponent>(it1->first);
		for (auto it2 = it1 + 1; it2 < colliders.end(); it2++) {
			auto &t2 = reg->get<TransformComponent>(it2->first);
			if (it1->second->CheckCollision(*it2->second, t1, t2, &m))
				detectedCollisions.push_back({
					{reg,it1->first},
					{ reg,it2->first },
					m }
				);
		}
	}
	return detectedCollisions;
}

};