#ifndef __RUSH_EVENT_H__
#define __RUSH_EVENT_H__

#include <string>

#include "Rush/core/Core.h"

namespace Rush { namespace Events {

enum class EventType{
    WindowOpen, WindowClose, WindowResize, WindowMove, WindowFocus,
    MouseMove, MouseScroll, MousePressed, MouseReleased,
    KeyboardPress, KeyboardRelease, KeyboardRepeat,
    ApplicationStart, ApplicationStop
};

enum EventGroup{
    WindowEvent = 0x1,
    MouseEvent = 0x2,
    KeyboardEvent = 0x4,
    ApplicationEvent = 0x8
};
    
#define EVENT_BODY(x) static EventType GetStaticType() { return EventType::x; } \
    virtual std::string GetName() const { return #x; }

class RUSH_API Event {
protected:
    bool m_IsHandled = false;
public:

    static EventType GetStaticType();
    virtual std::string GetName() const = 0;
    virtual std::string GetString() const { return GetName(); }
    EventType GetEventType() { return GetStaticType(); };
};
} }
#endif