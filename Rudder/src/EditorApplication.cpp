#include <Rush.h>
#include <Rush/core/EntryPoint.h>

#include "EditorLayer.h"

using namespace Rush;

class EditorApplication : public Application {
    virtual void Init() override {
        m_Window->SetVSync(true);
        PushOverlay(new EditorLayer());
    }
    virtual void Exit() override { }
    virtual void Update() override { }
};

Application *Rush::CreateApplication() {
    return new EditorApplication();
}