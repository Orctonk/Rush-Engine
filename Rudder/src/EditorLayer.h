#ifndef __EDITORLAYER_H__
#define __EDITORLAYER_H__

#include <Rush.h>
#include "EntityEditorWidget.h"

class EditorLayer : public Rush::Layer {
private:
    Rush::Shared<Rush::Shader> m_MeshShader;
    Rush::Shared<Rush::Shader> m_LightingShader;
    Rush::Shared<Rush::Shader> m_LightBoxShader;
    Rush::Shared<Rush::Texture> m_Texture;
    Rush::Unique<Rush::Framebuffer> m_GBuffer;
    Rush::Camera m_EditorCamera;
    entt::entity m_SelectedEnt;
    EntityEditor m_EE;
    bool m_Renaming;

    void RenderEntity(const entt::entity e);
    bool KeyPressHandle(Rush::KeyboardPressEvent &e);
public:
    EditorLayer();
    ~EditorLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnEvent(Rush::Event &e) override;
    virtual void OnImguiRender() override;
};

#endif // __EDITORLAYER_H__
