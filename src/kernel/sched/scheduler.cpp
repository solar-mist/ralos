#include <kernel/sched/scheduler.hpp>
#include <mem/pmm.hpp>
#include <drivers/ui/terminal.hpp>

extern "C" void JumpUsermode(void (*func)(), void* stack);

namespace Scheduler
{
    Task::Task(void* entry)
    {
        rip = entry;
        if(entry != nullptr)
        {
            rsp = PMM::GetPages(0x20000 / PAGE_SIZE); // TODO: Add virtual memory allocation and allocate virtual memory here
            rbp = rsp;
            stackSize = 0x20000;
        }
    }

    TaskQueue::TaskQueue()
    {
        m_Data = (Task*)PMM::GetPages(2);
        m_Size = 0;
        m_Capacity = (2 * PAGE_SIZE) / sizeof(Task);

        m_Front = m_Rear = m_Data;
    }

    void TaskQueue::Enqueue(Task t)
    {
        if(m_Size > ((2 * PAGE_SIZE) / sizeof(Task)))
        {
            Terminal::Print("Scheduler error: too many processes", 0xFF0000);
            asm volatile("cli; hlt"); // TODO: Kernel panic/allocate memory
        }
        
        *m_Rear = t;
        m_Rear++;
        m_Size++;
    }

    Task TaskQueue::Dequeue()
    {
        Task ret = *m_Front;
        m_Size--;
        m_Rear--;
        if(m_Size > 0)
            for(Task* t = m_Front; t < m_Rear; t++)
                *t = t[1];

        return ret;
    }

    Task* TaskQueue::Front()
    {
        return m_Front;
    }

    uint32_t TaskQueue::Size()
    {
        return m_Size;
    }

    static TaskQueue queue = TaskQueue();
    static int quantum = 30;
    static int timer = 0;
    static bool locked = true;

    void Init()
    {
        void (*start)() = (void(*)())queue.Front()->rip;
        locked = false;
        JumpUsermode(start, queue.Front()->rsp);
    }

    void IncrementTimer(ISRXFrame* frame)
    {
        timer++;
        if(timer == quantum)
        {
            if(queue.Size() != 1)
            {
                Task current = queue.Dequeue();
                current.rip = (void*)frame->BaseFrame.rip;
                current.rsp = (void*)frame->BaseFrame.rsp;
                current.rbp = (void*)frame->BaseFrame.rbp;
                queue.Enqueue(current);

                frame->BaseFrame.rip = (uint64_t)queue.Front()->rip;
                frame->BaseFrame.rsp = (uint64_t)queue.Front()->rsp;
                frame->BaseFrame.rbp = (uint64_t)queue.Front()->rbp;
            }
            timer = 0;
        }
    }

    void AddTask(Task t)
    {
        queue.Enqueue(t);
    }

    void EndCurrentTask(ISRXFrame* frame)
    {
        timer = 0;
        Task current = queue.Dequeue();
        PMM::FreePages(current.rsp, current.stackSize / PAGE_SIZE);
        Terminal::PrintInt((uint64_t)queue.Front()->rip, 10, 0x00FFFF);
        frame->BaseFrame.rip = (uint64_t)queue.Front()->rip;
        frame->BaseFrame.rsp = (uint64_t)queue.Front()->rsp;
        frame->BaseFrame.rbp = (uint64_t)queue.Front()->rbp;
    }

    Task* CurrentTask()
    {
        return queue.Front();
    }
}