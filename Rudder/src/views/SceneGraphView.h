#ifndef __SCENEGRAPHVIEW_H__
#define __SCENEGRAPHVIEW_H__

#include <Rush.h>
#include "widgets/EntityEditorWidget.h"

class SceneGraphView {
private:
    Rush::Scene *m_SceneRenderAccess;   // TODO: Remove this hack
    Rush::Scene::EntityType m_SelectedEnt;
    EntityEditor m_EE;
    bool m_Renaming;
    bool m_EEVisible;


    void RenderEntity(const Rush::Scene::EntityType e);
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
