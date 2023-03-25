#ifndef DRIVERS_PORTS_HPP
#define DRIVERS_PORTS_HPP
#include <libk/misc.hpp>

template<typename DataT>
class Port
{
public:
    Port(unsigned short port)
        :m_Port(port)
    {
    }


    DataT Read()
    {
        DataT result;
        if(is_same<DataT, unsigned char>::value)
            asm volatile("inb %1, %0" : "=a"(result) : "Nd"(m_Port));
        else if(is_same<DataT, unsigned short>::value)
            asm volatile("inw %1, %0" : "=a"(result) : "Nd"(m_Port));
        return result;
    }

    void Write(DataT data)
    {
        if(is_same<DataT, unsigned char>::value)
            asm volatile("outb %0, %1" :: "a"(data),  "Nd"(m_Port));
        else if(is_same<DataT, unsigned short>::value)
            asm volatile("outw %0, %1" :: "a"(data),  "Nd"(m_Port));
    }
private:
    unsigned short m_Port;
};

[[maybe_unused]] static inline void IOWait()
{
    Port<unsigned char> port = 0x80;
    port.Read();
}

#endif