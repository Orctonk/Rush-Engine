#ifndef __RUSH_EVENT_MANAGER_H__
#define __RUSH_EVENT_MANAGER_H__

#include "Rushpch.h"

#include "Rush/core/Core.h"
#include "Event.h"

namespace Rush { 

class RUSH_API EventQueue {
private:
    std::queue<Event *> m_EventQueue;
    std::mutex m_EventLock;
    
    EventQueue(/* args */) 
    :   m_EventQueue(), m_EventLock()
    { }
public:

    static EventQueue &GetInstance(){
        static EventQueue s_Instance;

        return s_Instance;
    }

    void PostEvent(Event *event){
        m_EventLock.lock();
        m_EventQueue.push(event);
        m_EventLock.unlock();
    }

    Event *ConsumeEvent(){
        Event *e = nullptr;
        m_EventLock.lock();
        if(!m_EventQueue.empty()){
            e = m_EventQueue.front();
            m_EventQueue.pop();
        }
        m_EventLock.unlock();
        return e;
    }
};

} 

#endif