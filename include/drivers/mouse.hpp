#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <common/types.hpp>
#include <hardware_communications/irq.hpp>
#include <hardware_communications/port.hpp>
#include <user_interface/display.hpp>
#include <drivers/driver.hpp>

class MouseDriver final : public InterruptHandler, public Driver 
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

    void activate() override;
};

#endif                                                                                                                                                      
