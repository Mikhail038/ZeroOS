#include "types.hpp"

template <typename data_size_t>
class PortSlow 
{
private:
    uint16_t number;

public:
    PortSlow(uint16_t number_) : number(number_) {};

    ~PortSlow() {};

    void        write(data_size_t data);
    data_size_t read();
};

template <>
void PortSlow<uint8_t>::write(uint8_t data)
{
    __asm__ volatile("outb %1, %0\njmp 1f\n1: jmp 1f\n1:": : "a" (data), "Nd" (number));
}

template <>
uint8_t PortSlow<uint8_t>::read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0": "=a" (result) : "Nd" (number));

    return result;
}