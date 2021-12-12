#include "EventQueue.h"
#include "Rushpch.h"

namespace Rush {

EventQueue::EventQueue(/* args */)
    : m_EventQueue(), m_EventLock() {
}

void EventQueue::PostEvent(Event *event) {
    RUSH_PROFILE_FUNCTION();
    m_EventLock.lock();
    m_EventQueue.push(event);
    m_EventLock.unlock();
}

Event *EventQueue::ConsumeEvent() {
    RUSH_PROFILE_FUNCTION();
    Event *e = nullptr;
    m_EventLock.lock();
    if (!m_EventQueue.empty()) {
        e = m_EventQueue.front();
        m_EventQueue.pop();
    }
    m_EventLock.unlock();
    return e;
}

} // namespace Rush
