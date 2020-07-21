#ifndef __EDITORLAYER_H__
#define __EDITORLAYER_H__

#include <Rush.h>
#include "views/RenderViews.h"
#include "views/SceneGraphView.h"
#include "views/AssetView.h"

class EditorLayer : public Rush::Layer {
private:
    Rush::Scene m_Scene;
    RenderViews m_RenderViews;
    SceneGraphView m_SceneGraph;
    AssetView m_AssetView;
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
