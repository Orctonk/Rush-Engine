#include "SandboxLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace Rush;

void SandboxLayer::OnAttach() {
	m_Step = false;
	m_Camera = m_Scene.NewEntity("Camera");
	m_Camera.GetComponent<TransformComponent>().Translate({ 0.0f,1.0f,35.0f });
	float width = Application::GetInstance().GetWindow()->GetWidth();
	float height = Application::GetInstance().GetWindow()->GetHeight();
	float aspect = width / height;
	Camera c(aspect, 45.0f);
	auto& cc = m_Camera.AddComponent<CameraComponent>(true, c);
	cc.clearColor = { 0.0f,0.4f,0.6f,1.0f };
	m_CC.SetControlledCamera(m_Camera);

	Shared<Material> mat = CreateShared<Material>();
	mat->materialShader = AssetManager::GetShader("res/shaders/materialShader.glsl");
	mat->diffuseTexture = AssetManager::GetTexture("res/textures/container2.png");

	/*Entity s1 = m_Scene.NewEntity("Sphere 1");
	Entity s2 = m_Scene.NewEntity("Sphere 2");
	Entity s3 = m_Scene.NewEntity("Sphere 3");
	s1.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/UVSphere.obj")).mesh->submeshes[0].material = mat;
	s1.AddComponent<SphereCollider>().radius = 1.5f;
	s1.AddComponent<Rigidbody>(1.0f, 0.8f);
	s2.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/UVSphere.obj"));
	s2.AddComponent<SphereCollider>().radius = 1.0f;
	s2.AddComponent<Rigidbody>(1.0f, 0.8f);
	s3.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/UVSphere.obj"));
	s3.AddComponent<SphereCollider>().radius = 1.0f;
	s3.AddComponent<Rigidbody>(1.0f, 0.8f);

	auto& tc1 = s1.GetComponent<TransformComponent>();
	auto& tc2 = s2.GetComponent<TransformComponent>();
	auto& tc3 = s3.GetComponent<TransformComponent>();
	tc1.SetScale(glm::vec3(1.5f));
	tc2.SetTranslation({ 1.0f,5.0f,0.0 });
	tc3.SetTranslation({ 4.0f,9.0f,0.0 });*/

	Entity b1 = m_Scene.NewEntity("Box 1");
	b1.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/cube.obj"));
	auto& bbox = b1.AddComponent<BoxCollider>();
	bbox.size = glm::vec3(100.0f, 2.0f, 100.0f);
	b1.AddComponent<Rigidbody>(0.0f, 0.8f);
	auto& btc1 = b1.GetComponent<TransformComponent>();
	btc1.Translate({ 0.0f,-6.0f,0.0f });
	btc1.SetScale({ 50.0f,1.0f,50.0f });

	Entity b2 = m_Scene.NewEntity("Box 2");
	b2.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/cube.obj"));
	auto& bbox2 = b2.AddComponent<BoxCollider>();
	bbox2.size = glm::vec3(2.0f, 2.0f, 2.0f);
	b2.AddComponent<Rigidbody>(0.0f, 0.8f);
	auto& btc2 = b2.GetComponent<TransformComponent>();
	btc2.Translate({ 0.0f,0.0f,0.0f });
	btc2.Rotate(glm::toQuat(glm::eulerAngleYX(0.785f, 0.785f)));

	Entity b5 = m_Scene.NewEntity("Box 5");
	b5.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/cube.obj"));
	auto& bbox5 = b5.AddComponent<BoxCollider>();
	bbox5.size = glm::vec3(2.0f, 2.0f, 2.0f);
	b5.AddComponent<Rigidbody>(1.0f, 0.8f);
	auto& btc5 = b5.GetComponent<TransformComponent>();
	btc5.Translate({ 0.0f,6.0f,0.0f });
	btc5.Rotate(glm::toQuat(glm::eulerAngleZ(0.785f)));

	Entity b3 = m_Scene.NewEntity("Box 3");
	b3.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/cube.obj"));
	auto& bbox3 = b3.AddComponent<BoxCollider>();
	bbox3.size = glm::vec3(2.0f, 10.0f, 20.0f);
	b3.AddComponent<Rigidbody>(0.0f, 0.8f);
	auto& btc3 = b3.GetComponent<TransformComponent>();
	btc3.Translate({ -10.0f,2.0f,0.0f });
	btc3.SetScale({ 1.0f, 5.0f, 10.0f});

	Entity b4 = m_Scene.NewEntity("Box 4");
	b4.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/cube.obj"));
	auto& bbox4 = b4.AddComponent<BoxCollider>();
	bbox4.size = glm::vec3(2.0f, 10.0f, 20.0f);
	b4.AddComponent<Rigidbody>(0.0f, 0.8f);
	auto& btc4 = b4.GetComponent<TransformComponent>();
	btc4.Translate({ 10.0f,2.0f,0.0f });
	btc4.SetScale({ 1.0f, 5.0f, 10.0f });

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
	if (Input::KeyDown(RUSH_KEY_P) || m_Step) {
		PhysicsSystem::StepPhysics(m_Scene);
		m_AccumulatedTime -= Time::FixedDelta();
		m_Step = false;
	}
	m_Scene.Render();
	PhysicsSystem::DrawDebug(m_Scene);
}

void SandboxLayer::OnEvent(Event& e){
	m_CC.OnEvent(e);
	e.Dispatch<KeyboardPressEvent>(RUSH_BIND_FN(SandboxLayer::OnKeyDown));
}

bool SandboxLayer::OnKeyDown(Rush::KeyboardPressEvent& e) {
	if (e.keycode == RUSH_KEY_S) {
		m_Step = true;
		return true;
	}
	return false;
}