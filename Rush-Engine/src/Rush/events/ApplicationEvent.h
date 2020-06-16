#ifndef __APPLICATIONEVENT_H__
#define __APPLICATIONEVENT_H__
#include "Event.h"

namespace Rush{ 

class ApplicationStartEvent : public Event{
public:
    ApplicationStartEvent() {}
    EVENT_BODY(ApplicationStart);
    virtual std::string GetString() const override { return "ApplicationStartEvent"; }
};

class ApplicationStopEvent : public Event{
public:
    ApplicationStopEvent() {}
    EVENT_BODY(ApplicationStop);
    virtual std::string GetString() const override { return "ApplicationStopEvent"; }
};

} 
#endif // __APPLICATIONEVENT_H__
