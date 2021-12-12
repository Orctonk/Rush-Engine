#ifndef __SCENEGRAPHVIEW_H__
#define __SCENEGRAPHVIEW_H__

#include "widgets/EntityEditorWidget.h"
#include <Rush.h>

class SceneGraphView {
private:
    EntityEditor m_EE;
    bool m_Renaming;
    bool m_EEVisible;
    bool m_Focused;

    void RenderEntity(const Rush::Entity e);
    bool KeyPressHandle(Rush::KeyboardPressEvent &e);

public:
    bool enabled;

    SceneGraphView();
    ~SceneGraphView();

    void OnEvent(Rush::Event &e);
    void OnImguiRender(Rush::Scene &scene);

    static void RenderMaterial(Rush::Material &mat);
};

#endif // __SCENEGRAPHVIEW_H__
