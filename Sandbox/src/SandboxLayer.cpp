#include "SandboxLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace Rush;

void SandboxLayer::OnAttach() {
	m_Camera = m_Scene.NewEntity("Camera");
	m_Camera.GetComponent<TransformComponent>().Translate({ 0.0f,1.0f,10.0f });
	float width = Application::GetInstance().GetWindow()->GetWidth();
	float height = Application::GetInstance().GetWindow()->GetHeight();
	float aspect = width/height;
	Camera c(aspect, 90.0f);
	auto &cc = m_Camera.AddComponent<CameraComponent>(true, c);
	cc.clearColor = { 0.0f,0.4f,0.6f,1.0f };

	Entity s1 = m_Scene.NewEntity("Sphere 1");
	Entity s2 = m_Scene.NewEntity("Sphere 2");
	Entity s3 = m_Scene.NewEntity("Sphere 3");
	s1.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/UVSphere.obj"));
	s1.AddComponent<SphereCollider>().radius = 1.5f;
	s1.AddComponent<Rigidbody>(0.0f,0.5f);
	s2.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/UVSphere.obj"));
	s2.AddComponent<SphereCollider>().radius = 1.0f;
	s2.AddComponent<Rigidbody>(1.0f,0.5f);
	s3.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/UVSphere.obj"));
	s3.AddComponent<SphereCollider>().radius = 1.0f;
	s3.AddComponent<Rigidbody>(1.0f,0.5f);

	auto& tc1 = s1.GetComponent<TransformComponent>();
	auto& tc2 = s2.GetComponent<TransformComponent>();
	auto& tc3 = s3.GetComponent<TransformComponent>();
	tc1.SetScale(glm::vec3(1.5f));
	tc2.SetTranslation({1.0f,5.0f,0.0});
	tc3.SetTranslation({4.0f,9.0f,0.0});

	Entity b1 = m_Scene.NewEntity("Box 1");
	b1.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/cube.obj"));
	auto &bbox = b1.AddComponent<AABBCollider>();
	bbox.min = glm::vec3(-10.0f, -1.0f, -10.0f);
	bbox.max = glm::vec3(10.0f, 1.0f, 10.0f);
	b1.AddComponent<Rigidbody>(0.0f, 0.5f);
	auto& btc1 = b1.GetComponent<TransformComponent>();
	btc1.Translate({ 0.0f,-6.0f,0.0f });
	btc1.SetScale({ 10.0f,1.0f,10.0f });

	Entity b2 = m_Scene.NewEntity("Box 2");
	b2.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/cube.obj"));
	auto& bbox2 = b2.AddComponent<AABBCollider>();
	b2.AddComponent<Rigidbody>(1.0f, 0.5f);
	auto& btc2 = b2.GetComponent<TransformComponent>();
	btc2.Translate({ -4.0f,6.0f,0.0f });

	auto l1 = m_Scene.NewEntity("Light 1");
	auto& lt1 = l1.GetComponent<TransformComponent>();
	lt1.Translate({ 15.0f,7.0f,7.0f });
	LightComponent& lc1 = l1.AddComponent<LightComponent>();
	lc1.ambient = glm::vec3(0.1f);
	lc1.diffuse = glm::vec3(1.0f);
	lc1.specular = glm::vec3(1.0f);

	lc1.constant = 1.0f;
	lc1.linear = .02f;
	lc1.quadratic = .008f;

	auto l2 = m_Scene.NewEntity("Light 2");
	auto& lt2 = l2.GetComponent<TransformComponent>();
	lt2.Translate({ -15.0f,7.0f,7.0f });
	LightComponent& lc2 = l2.AddComponent<LightComponent>();
	lc2.ambient = glm::vec3(0.1f);
	lc2.diffuse = glm::vec3(1.0f);
	lc2.specular = glm::vec3(1.0f);

	lc2.constant = 1.0f;
	lc2.linear = .02f;
	lc2.quadratic = .008f;

	PhysicsSystem::SetGravity(9.2f);
	m_AccumulatedTime = 0.0f;
}

void SandboxLayer::OnUpdate() {
	m_AccumulatedTime += Time::Delta();
	while (m_AccumulatedTime > Time::FixedDelta()) {
		PhysicsSystem::StepPhysics(m_Scene);
		m_AccumulatedTime -= Time::FixedDelta();
	}
	m_Scene.Render();

	auto collisions = CollisionDetection::DetectCollisions(m_Scene);
	for (auto c : collisions) {
		std::string e1name = c.e1.GetComponent<TagComponent>().tag;
		std::string e2name = c.e2.GetComponent<TagComponent>().tag;
		RUSH_LOG_INFO("Collision between " + e1name + " and " + e2name + "(" + std::to_string(c.manifold.penetrationDepth) + ")");
	}
}

void SandboxLayer::OnEvent(Event& e){
	e.Dispatch<WindowResizeEvent>(RUSH_BIND_FN(SandboxLayer::OnResize));
}

bool SandboxLayer::OnResize(WindowResizeEvent& e) {
	float aspect =  (float)e.width / (float)e.height;
	m_Camera.GetComponent<CameraComponent>().camera.SetPerspective(aspect,90.0f);
	return false;
}