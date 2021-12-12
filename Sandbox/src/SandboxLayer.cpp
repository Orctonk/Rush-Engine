#include "SandboxLayer.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace Rush;

void SandboxLayer::OnAttach() {
    m_Camera = m_Scene.NewEntity("Camera");
    m_Camera.GetComponent<TransformComponent>().Translate({ 0.0f, 1.0f, 3.0f });
    m_Camera.GetComponent<TransformComponent>().Rotate(glm::toQuat(glm::eulerAngleXYZ(90.0f, 0.0f, 0.0f)));
    float width = Application::GetInstance().GetWindow()->GetWidth();
    float height = Application::GetInstance().GetWindow()->GetHeight();
    float aspect = width / height;
    Camera c(aspect, 90.0f);
    auto &cc = m_Camera.AddComponent<CameraComponent>(true, c);
    cc.clearColor = { 0.0f, 0.4f, 0.6f, 1.0f };

    Entity b1 = m_Scene.NewEntity("Box 1");
    b1.AddComponent<MeshRendererComponent>(AssetManager::GetMesh("res/models/cube.obj"));
    auto &bt = b1.GetComponent<TransformComponent>();
    bt.SetScale({ 0.5, 0.5, 0.5 });

    Entity light = m_Scene.NewEntity("Light");
    LightComponent &l = light.AddComponent<LightComponent>();
    l.ambient = glm::vec3(0.1f);
    l.diffuse = glm::vec3(1.0f);
    l.specular = glm::vec3(1.0f);

    auto &lt = light.GetComponent<TransformComponent>();
    lt.SetTranslation({ 1.0f, 1.0f, 1.0f });
}

void SandboxLayer::OnUpdate() {
    float time = Time::ProgramTimeMillis() / 1000.0f;
    auto &t = m_Camera.GetComponent<TransformComponent>();
    t.SetTranslation({ sin(time) * 3.0f, 0.0f, cos(time) * 3.0f });
    t.SetRotation(glm::toQuat(glm::eulerAngleY(time)));
    m_Scene.Render();
}

void SandboxLayer::OnEvent(Event &e) {
    e.Dispatch<WindowResizeEvent>(RUSH_BIND_FN(SandboxLayer::OnResize));
}

bool SandboxLayer::OnResize(WindowResizeEvent &e) {
    float aspect = (float)e.width / (float)e.height;
    m_Camera.GetComponent<CameraComponent>().camera.SetPerspective(aspect, 90.0f);
    return false;
}