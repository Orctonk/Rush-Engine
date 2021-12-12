#ifndef __MOUSEEVENT_H__
#define __MOUSEEVENT_H__
#include "Event.h"

namespace Rush {

class MouseMoveEvent : public Event {
public:
    int x, y;

    MouseMoveEvent(int x, int y)
        : x(x), y(y) { }
    EVENT_BODY(MouseMove);
    virtual std::string GetString() const override { return "MouseMoveEvent(" + std::to_string(x) + "," + std::to_string(y) + ")"; }
};

class MouseScrollEvent : public Event {
public:
    int delta;

    MouseScrollEvent(int delta)
        : delta(delta) { }
    EVENT_BODY(MouseScroll);
    virtual std::string GetString() const override { return "MouseScrollEvent(" + std::to_string(delta) + ")"; }
};

class MousePressedEvent : public Event {
public:
    int keycode;

    MousePressedEvent(int keycode)
        : keycode(keycode) { }
    EVENT_BODY(MousePressed);
    virtual std::string GetString() const override { return "MousePressedEvent(" + std::to_string(keycode) + ")"; }
};

class MouseReleasedEvent : public Event {
public:
    int keycode;

    MouseReleasedEvent(int keycode)
        : keycode(keycode) { }
    EVENT_BODY(MouseReleased);
    virtual std::string GetString() const override { return "MouseReleasedEvent(" + std::to_string(keycode) + ")"; }
};

} // namespace Rush
#endif // __MOUSEEVENT_H__
