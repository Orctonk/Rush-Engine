#ifndef __RUSH_EVENT_MANAGER_H__
#define __RUSH_EVENT_MANAGER_H__

#include <queue>
#include <mutex>
#include <unordered_map>
#include <functional>

#include "../core/Core.h"
#include "Event.h"

namespace Rush { namespace Events {

class AbstractHandler{
public:
    virtual bool HandleEvent(Event &e) = 0;
};

template<typename EventT>
class EventHandler : public AbstractHandler{
private:
    typedef std::function<bool(EventT)> EventFn;
    EventFn m_HandlerFn;
public:
    EventHandler(EventFn handler){
        m_HandlerFn = handler;
    }

    bool HandleEvent(Event &e) override{
        return m_HandlerFn((EventT &)e);
    }
};

class RUSH_API EventManager {
private:
    std::unordered_map<EventType,std::vector<AbstractHandler *>> handlermap;
    std::queue<Event *> m_EventQueue;
    std::mutex m_EventLock;
    
    EventManager(/* args */) 
    :   handlermap()
    { }
public:

    static EventManager &GetInstance(){
        static EventManager s_Instance;

        return s_Instance;
    }

    template<typename EventT>
    void RegisterHandler(bool (*handler)(EventT)){
        static_assert(std::is_base_of<Event,EventT>());
        handlermap[EventT::GetStaticType()].push_back(new EventHandler<EventT>(handler));
    }

    template<typename EventT>
    void RegisterHandler(std::function<bool(EventT)> handler){
        static_assert(std::is_base_of<Event,EventT>());
        handlermap[EventT::GetStaticType()].push_back(new EventHandler<EventT>(handler));
    }

    template<typename EventT>
    void PostEvent(EventT event){
        static_assert(std::is_base_of<Event,EventT>());
        for(AbstractHandler *handler : handlermap[EventT::GetStaticType()]){
            if(handler->HandleEvent(event));
        }
    }
};

} }

#endif