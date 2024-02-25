#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "types.hpp"
#include "irq.hpp"
#include "port.hpp"
#include "port_slow.hpp"

#include "display.hpp"

class KeyboardDriver : public InterruptHandler
{
    const uint16_t keyboard_irq = 0x21;

    Port<uint8_t> data_port;
    Port<uint8_t> command_port;

public:
    KeyboardDriver(InterruptManager* manager);
    ~KeyboardDriver();

    virtual uint32_t handle_interrupt(uint32_t esp);
};

#endif