#ifndef __EDITORLAYER_H__
#define __EDITORLAYER_H__

#include <Rush.h>
#include "views/RenderViews.h"
#include "views/SceneGraphView.h"

class EditorLayer : public Rush::Layer {
private:
    Rush::Shared<Rush::Model> m_Model;
    RenderViews m_RenderViews;
    SceneGraphView m_SceneGraph;
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
