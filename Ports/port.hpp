#include "types.hpp"

template <typename data_size_t>
class Port 
{
private:
    uint16_t number;

public:
    Port(uint16_t number_) : number(number_) {};

    ~Port() {};

    void        write(data_size_t data) const;
    data_size_t read() const;
};

template <>
void Port<uint8_t>::write(uint8_t data) const
{
    __asm__ volatile("outb %0, %1": : "a" (data), "Nd" (number));
}

template <>
uint8_t Port<uint8_t>::read() const
{
    uint8_t result;
    __asm__ volatile("inb %1, %0": "=a" (result) : "Nd" (number));

    return result;
}

template <>
void Port<uint16_t>::write(uint16_t data) const
{
    __asm__ volatile("outw %0, %1": : "a" (data), "Nd" (number));
}

template <>
uint16_t Port<uint16_t>::read() const
{
    uint16_t result;
    __asm__ volatile("inw %1, %0": "=a" (result) : "Nd" (number));

    return result;
}

template <>
void Port<uint32_t>::write(uint32_t data) const
{
    __asm__ volatile("outl %0, %1": : "a" (data), "Nd" (number));
}

template <>
uint32_t Port<uint32_t>::read() const
{
    uint32_t result;
    __asm__ volatile("inl %1, %0": "=a" (result) : "Nd" (number));

    return result;
}