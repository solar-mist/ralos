#include <kernel/sched/scheduler.hpp>
#include <mm/pmm.hpp>
#include <mm/vmm.hpp>
#include <mm/kheap.hpp>
#include <fs/vfs.hpp>
#include <drivers/terminal.hpp>
#include <libk/mem.hpp>

extern "C" void enter_usermode(void (*func)(), void* stack, int auxval);

namespace Scheduler
{
    TaskQueue::TaskQueue()
    {
        m_Data = (Process*)PhysToVirt((uint64_t)PMM::GetPages(2));
        m_Size = 0;
        m_Capacity = (2 * PAGE_SIZE) / sizeof(Process);

        m_Front = m_Rear = m_Data;
    }

    void TaskQueue::Enqueue(Process t)
    {
        if(m_Size >= m_Capacity)
        {
            Terminal::Print("Scheduler error: too many processes", 0xFF0000);
            asm volatile("cli; hlt"); // TODO: Kernel panic/allocate memory
        }
        
        *m_Rear = t;
        m_Rear++;
        m_Size++;
    }

    Process TaskQueue::Dequeue()
    {
        Process ret = *m_Front;
        m_Size--;
        m_Rear--;
        if(m_Size > 0)
            for(Process* t = m_Front; t < m_Rear; t++)
                *t = t[1];

        return ret;
    }

    Process* TaskQueue::Front()
    {
        return m_Front;
    }

    uint32_t TaskQueue::Size()
    {
        return m_Size;
    }

    static TaskQueue queue;
    static int quantum = 25;
    static int timer = 0;
    static bool locked = true;

    void Init()
    {
        queue = TaskQueue();
    }

    void Start()
    {
        void (*start)() = (void(*)())queue.Front()->state.BaseFrame.rip;
        locked = false;
        Paging::SwitchAddrSpace(&queue.Front()->addrspace);
        int fd = 0;
        if(queue.Front()->interpPath)
        {
            VFS::Node* node = (VFS::Node*)malloc(sizeof(VFS::Node));
            VFS::GetFileSystem("tmpfs")->lookup(queue.Front()->interpPath, node);
            queue.Front()->fdTable[3] = ProcFile{ node, 0, 1 };
            fd = 3;
        }
        enter_usermode(start, (void*)queue.Front()->state.BaseFrame.rsp, fd);
    }

    void SwitchCtx(InterruptFrame* frame, Process* newProc)
    {
        memcpy(&frame->GeneralRegisters, &newProc->state.GeneralRegisters, sizeof(InterruptFrame::GeneralRegisters));
        frame->BaseFrame.rip = newProc->state.BaseFrame.rip;
        frame->BaseFrame.rsp = newProc->state.BaseFrame.rsp;
        frame->BaseFrame.rbp = newProc->state.BaseFrame.rbp;
        frame->ControlRegisters.cr3 = newProc->state.ControlRegisters.cr3;
    }

    void IncrementTimer(InterruptFrame* frame)
    {
        if(!locked)
            timer++;
        if(timer == quantum)
        {
            if(queue.Size() != 1)
            {
                Process current = queue.Dequeue();
                memcpy(&current.state.GeneralRegisters, &frame->GeneralRegisters, sizeof(InterruptFrame::GeneralRegisters));
                current.state.BaseFrame.rip = frame->BaseFrame.rip;
                current.state.BaseFrame.rsp = frame->BaseFrame.rsp;
                current.state.BaseFrame.rbp = frame->BaseFrame.rbp;
                current.state.ControlRegisters.cr3 = frame->ControlRegisters.cr3;
                queue.Enqueue(current);

                SwitchCtx(frame, queue.Front());
            }
            timer = 0;
        }
    }

    void AddProcess(Process t)
    {
        VFS::Node* stdout = (VFS::Node*)malloc(sizeof(VFS::Node));
        VFS::GetFileSystem("tmpfs")->lookup("/stdout", stdout);
        t.fdTable[1] = ProcFile{ stdout, 0, 2 };
        queue.Enqueue(t);
    }

    void EndCurrentProcess(InterruptFrame* state)
    {
        Process current = queue.Dequeue();
        VMM::FreePages(&current.addrspace, (void*)current.stack, current.stackSize);

        Paging::SwitchAddrSpace(&queue.Front()->addrspace);
        SwitchCtx(state, queue.Front());
        timer = 0;
    }

    Process* CurrentProcess()
    {
        return queue.Front();
    }

    uint32_t RunningProcesses()
    {
        return queue.Size();
    }
}