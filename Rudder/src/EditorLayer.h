#ifndef __EDITORLAYER_H__
#define __EDITORLAYER_H__

#include <Rush.h>
#include <imgui_entt_entity_editor.hpp>

class EditorLayer : public Rush::Layer {
private:
    MM::EntityEditor<entt::entity> m_EntEditor;
    entt::entity m_SelectedEnt;

    void RenderEntity(entt::registry &reg,const entt::entity e);
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
