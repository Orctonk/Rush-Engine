#ifndef __IMGUILAYER_H__
#define __IMGUILAYER_H__

#include "Rush/core/Layer.h"
#include "Rush/events/KeyboardEvent.h"
#include "Rush/events/MouseEvent.h"
#include "Rush/events/WindowEvent.h"

namespace Rush {

class RUSH_API ImguiLayer : public Layer {
private:
    bool ImguiKeyPressHandle(KeyboardPressEvent &e);
    bool ImguiKeyReleaseHandle(KeyboardReleaseEvent &e);
    bool ImguiKeyTypeHandle(KeyboardTypeEvent &e);
    bool ImguiMouseScrollHandle(MouseScrollEvent &e);
    bool ImguiMouseMoveHandle(MouseMoveEvent &e);
    bool ImguiMousePressHandle(MousePressedEvent &e);
    bool ImguiMouseReleaseHandle(MouseReleasedEvent &e);
    bool ImguiWindowResizeHandle(WindowResizeEvent &e);

public:
    ImguiLayer();
    ~ImguiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImguiRender() override { }
    virtual void OnEvent(Event &e) override;

    void Begin();
    void End();
};

} // namespace Rush

#endif // __IMGUILAYER_H__
