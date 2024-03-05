#ifndef PORT_HPP
#define PORT_HPP

#include "types.hpp"

template <typename data_size_t>
class Port final
{
private:
    uint16_t number;

public:
    Port(uint16_t number_) : number(number_) {};

    ~Port() = default;

    void        write(data_size_t data) const;
    data_size_t read() const;
};

template <>
class Port<uint8_t>
{
    private:
    uint16_t number;

public:
    Port(uint16_t number_) : number(number_) {};

    ~Port() = default;

    void write(uint8_t data) const
    {
        __asm__ volatile("outb %0, %1": : "a" (data), "Nd" (number));
    }

    uint8_t read() const
    {
        uint8_t result;
        __asm__ volatile("inb %1, %0": "=a" (result) : "Nd" (number));

        return result;
    }
};

template <>
class Port<uint16_t>
{
private:
    uint16_t number;

public:
    Port(uint16_t number_) : number(number_) {};

    ~Port() = default;

    void write(uint16_t data) const
    {
        __asm__ volatile("outw %0, %1": : "a" (data), "Nd" (number));
    }

    uint16_t read() const
    {
        uint16_t result;
        __asm__ volatile("inw %1, %0": "=a" (result) : "Nd" (number));

        return result;
    }
};

template <>
class Port<uint32_t>
{
private:
    uint16_t number;

public:
    Port(uint16_t number_) : number(number_) {};

    ~Port() = default;

    void write(uint32_t data) const
    {
        __asm__ volatile("outl %0, %1": : "a" (data), "Nd" (number));
    }

    uint32_t read() const
    {
        uint32_t result;
        __asm__ volatile("inl %1, %0": "=a" (result) : "Nd" (number));

        return result;
    }
};

#endif