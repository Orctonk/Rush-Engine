#ifndef __RUSH_EVENT_H__
#define __RUSH_EVENT_H__

#include <string>

#include "Rushpch.h"
#include "Rush/core/Core.h"

#define RUSH_BIND_FN(x) (std::bind(&x,this,std::placeholders::_1))

namespace Rush { 

enum class EventType{
    WindowOpen, WindowClose, WindowResize, WindowMove, WindowFocus,
    MouseMove, MouseScroll, MousePressed, MouseReleased,
    KeyboardPress, KeyboardRelease, KeyboardRepeat, KeyboardType,
    ApplicationStart, ApplicationStop
};

enum EventGroup{
    WindowEvent = 0x1,
    MouseEvent = 0x2,
    KeyboardEvent = 0x4,
    ApplicationEvent = 0x8
};
    
#define EVENT_BODY(x) static EventType GetStaticType() { return EventType::x; } \
    virtual std::string GetName() const override { return #x; } \
    virtual EventType GetEventType() const override { return GetStaticType(); }

class RUSH_API Event {
protected:
    bool m_IsHandled = false;
public:

    bool IsHandled() { return m_IsHandled; }

    static EventType GetStaticType();
    virtual std::string GetName() const = 0;
    virtual std::string GetString() const { return GetName(); }
    virtual EventType GetEventType() const = 0;

    template<typename EventT, typename EventF>
    void Dispatch(const EventF &eventFn){
        if(GetEventType() == EventT::GetStaticType()){
            m_IsHandled = eventFn(static_cast<EventT&>(*this));
        }
    }
};

} 
#endif