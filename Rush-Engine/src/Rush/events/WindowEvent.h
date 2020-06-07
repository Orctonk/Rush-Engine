#ifndef __WINDOWEVENT_H__
#define __WINDOWEVENT_H__
#include "Event.h"

namespace Rush{ namespace Events{

class RUSH_API WindowOpenEvent : public Event{
public:
    WindowOpenEvent(){}
    EVENT_BODY(WindowOpen);
};

class RUSH_API WindowCloseEvent : public Event{
public:
    WindowCloseEvent(){}
    EVENT_BODY(WindowClose);
};

class RUSH_API WindowResizeEvent : public Event{
public:
    int width, height;

    WindowResizeEvent(int width, int height)
    : width(width), height(height) {}

    EVENT_BODY(WindowResize);
    virtual std::string GetString() const override {
        return "WindowResizeEvent(" + std::to_string(width) + "," + std::to_string(height) + ")";  
    }
};

class WindowMoveEvent : public Event{
public:
    int x, y;

    WindowMoveEvent(int x, int y)
    : x(x), y(y) {}

    EVENT_BODY(WindowMove);
    virtual std::string GetString() const override {
        return "WindowMoveEvent(" + std::to_string(x) + "," + std::to_string(y) + ")";  
    }
};

class WindowFocusEvent : public Event{
public:
    bool gained;

    WindowFocusEvent(bool gained)
    : gained(gained) {}

    EVENT_BODY(WindowFocus);
    virtual std::string GetString() const override {
        return std::string("WindowFocusEvent(") + (gained ? "gained" : "lost")  + ")";  
    }
};

} }
#endif // __WINDOWEVENT_H__
