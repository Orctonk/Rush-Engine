#ifndef __RUSH_EVENT_MANAGER_H__
#define __RUSH_EVENT_MANAGER_H__

#include "Event.h"
#include "Rush/core/Core.h"

#include <mutex>
#include <queue>

namespace Rush {

class RUSH_API EventQueue {
private:
    std::queue<Event *> m_EventQueue;
    std::mutex m_EventLock;

    EventQueue();

public:
    static EventQueue &GetInstance() {
        static EventQueue s_Instance;

        return s_Instance;
    }

    void PostEvent(Event *event);
    Event *ConsumeEvent();
};

} // namespace Rush

#endif