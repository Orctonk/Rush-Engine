#ifndef _RUSH_QUEUE_H_
#define _RUSH_QUEUE_H_

namespace Rush{

    template<typename T>
    class Queue
    {
    public:
        Queue();
        ~Queue();

        virtual void enqueue(T);
        virtual T dequeue();
        virtual int size();
        virtual bool empty(); 
    };
} 

#endif // _RUSH_QUEUE_H_