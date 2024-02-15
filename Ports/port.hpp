#include "types.hpp"

template <typename data_size_t>
class Port 
{
private:
    uint16_t number;

public:
    Port(uint16_t number_) : number(number_) {};

    ~Port() {};

    void        write(data_size_t data);
    data_size_t read();
};

template <>
void Port<uint8_t>::write(uint8_t data)
{
    __asm__ volatile("outb %1, %0": : "a" (data), "Nd" (number));
}

template <>
uint8_t Port<uint8_t>::read()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0": "=a" (result) : "Nd" (number));

    return result;
}

template <>
void Port<uint16_t>::write(uint16_t data)
{
    __asm__ volatile("outw %1, %0": : "a" (data), "Nd" (number));
}

template <>
uint16_t Port<uint16_t>::read()
{
    uint16_t result;
    __asm__ volatile("inw %1, %0": "=a" (result) : "Nd" (number));

    return result;
}

template <>
void Port<uint32_t>::write(uint32_t data)
{
    __asm__ volatile("outl %1, %0": : "a" (data), "Nd" (number));
}

template <>
uint32_t Port<uint32_t>::read()
{
    uint32_t result;
    __asm__ volatile("inl %1, %0": "=a" (result) : "Nd" (number));

    return result;
}