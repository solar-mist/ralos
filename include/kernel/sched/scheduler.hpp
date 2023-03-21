#ifndef KERNEL_SCHED_SCHEDULER_HPP
#define KERNEL_SCHED_SCHEDULER_HPP
#include <cpu/interrupt/isr.hpp>
#include <stdint.h>

namespace Scheduler
{
    struct Task
    {
        Task(void* entry);
        void* rip;
        void* rsp;
        void* rbp;
        uint64_t stackSize;
    };

    class TaskQueue
    {
    public:
        TaskQueue();
        Task Dequeue();
        void Enqueue(Task t);
        Task* Front();

        uint32_t Size();
    private:
        Task* m_Data;
        uint32_t m_Size;
        uint32_t m_Capacity;

        Task* m_Front,* m_Rear;
    };

    void Init();

    void Lock();
    void Unlock();

    void IncrementTimer(ISRXFrame* frame);

    void AddTask(Task t);
    void EndCurrentTask(ISRXFrame* frame);

    Task* CurrentTask();
}

#endif