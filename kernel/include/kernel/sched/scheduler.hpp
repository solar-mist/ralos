#ifndef KERNEL_SCHED_SCHEDULER_HPP
#define KERNEL_SCHED_SCHEDULER_HPP
#include <cpu/interrupt/isr.hpp>
#include <proc/process.hpp>
#include <stdint.h>

namespace Scheduler
{
    class TaskQueue
    {
    public:
        TaskQueue();
        Process Dequeue();
        void Enqueue(Process t);
        Process* Front();

        uint32_t Size();
    private:
        Process* m_Data;
        uint32_t m_Size;
        uint32_t m_Capacity;

        Process* m_Front,* m_Rear;
    };

    void Init();
    void Start();

    void IncrementTimer(InterruptFrame* frame);

    void AddTask(Process t);

    Process* CurrentTask();
}

#endif