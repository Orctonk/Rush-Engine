#ifndef __SCENEGRAPHVIEW_H__
#define __SCENEGRAPHVIEW_H__

#include <Rush.h>
#include "widgets/EntityEditorWidget.h"

class SceneGraphView {
private:
    entt::entity m_SelectedEnt;
    EntityEditor m_EE;
    bool m_Renaming;
    bool m_EEVisible;

    void RenderEntity(const entt::entity e);
    bool KeyPressHandle(Rush::KeyboardPressEvent &e);
public:
    bool enabled;

    SceneGraphView();
    ~SceneGraphView();

    void OnEvent(Rush::Event &e);
    void OnImguiRender();
};

#endif // __SCENEGRAPHVIEW_H__
