#include <proc/process.hpp>

Process::Process(uint64_t entry)
{
    frame.BaseFrame.rip = entry;
}