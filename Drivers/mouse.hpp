#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "types.hpp"
#include "irq.hpp"
#include "port.hpp"

#include "display.hpp"

class MouseDriver final : public InterruptHandler
{
    Display& display;

    Port<uint8_t> data_port;
    Port<uint8_t> command_port;

    uint8_t buffer[3];

    uint8_t offset;
    uint8_t buttons;

public:
    MouseDriver(InterruptManager* manager, Display& display_);
    ~MouseDriver() = default;

    uint32_t handle_interrupt(uint32_t esp) override;
};

#endif                                                                                                                                                      
