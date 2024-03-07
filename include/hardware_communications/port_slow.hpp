#ifndef SLOW_PORT_HPP
#define SLOW_PORT_HPP


#include <common/types.hpp>

void putline(char* line);

template <typename data_size_t>
class PortSlow final
{
private:
    uint16_t number;

public:
    PortSlow(uint16_t number_) : number(number_) {};

    ~PortSlow() {};

    void        write(data_size_t data) const;
    data_size_t read() const;
};

template <>
class PortSlow<uint8_t> 
{
private:
    uint16_t number;

public:
    PortSlow(uint16_t number_) : number(number_) {};

    ~PortSlow() {};

    void write(uint8_t data) const
    {
        __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:": : "a" (data), "Nd" (number));
    }

    uint8_t read() const
    {
        uint8_t result;
        __asm__ volatile("inb %1, %0": "=a" (result) : "Nd" (number));

        return result;
    }
};

#endif