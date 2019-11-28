#ifndef _RUSH_TSLINKEDQUEUE_H_
#define _RUSH_TSLINKEDQUEUE_H_

#include "Queue.h"
#include <mutex>
#include <memory>

#define RUSH_AQ_DEF_SIZE

namespace Rush
{
    template<typename T>
    class TSLinkedQueue : public Queue{
    private:
        struct node{
            std::smart_ptr<nextNode> next;
            T data;
        };

        node m_head;

        int m_Size;
        std::mutex m_EndLock;
        std::mutex m_FrontLock;
        
    public:
        TSLinkedQueue();
        ~TSLinkedQueue();

        void enqueue(T) override;
        T dequeue() override;
        int size() override;
        bool empty() override; 
    };

    template<typename T>
    TSLinkedQueue<T>::TSLinkedQueue() :
        m_Size(0),
        m_EndLock(),
        m_FrontLock() {
            
    }

    template<typename T>
    TSLinkedQueue<T>::~TSLinkedQueue() {}
} 


#endif // _RUSH_TSLINKEDQUEUE_H_