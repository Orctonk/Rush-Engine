#ifndef __EDITORLAYER_H__
#define __EDITORLAYER_H__

#include "views/AssetView.h"
#include "views/ExceptionPanel.h"
#include "views/ProfilerResultView.h"
#include "views/RenderViews.h"
#include "views/SceneGraphView.h"
#include "views/SceneView.h"
#include <Rush.h>

class EditorLayer : public Rush::Layer {
private:
    Rush::Scene m_Scene;
    Rush::Path m_ScenePath;

    RenderViews m_RenderViews;
    SceneGraphView m_SceneGraph;
    SceneView m_SceneView;
    AssetView m_AssetView;
    ProfilerResultView m_PRView;
    ExceptionPanel m_Exceptions;

    void AddImGuiFonts();

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
