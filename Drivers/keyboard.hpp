#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "types.hpp"
#include "irq.hpp"
#include "port.hpp"
#include "port_slow.hpp"
#include "display.hpp"
#include "driver.hpp"

class KeyboardDriver final : public InterruptHandler, public Driver 
{
    Display& display;

    bool shift_pressed;
    bool ctrl_pressed;

    Port<uint8_t> data_port;
    Port<uint8_t> command_port;

public:
    KeyboardDriver(InterruptManager* manager, Display& display_);
    ~KeyboardDriver() = default;

    uint32_t handle_interrupt(uint32_t esp) override;

    void activate() override;

private:
    void hex_print_key(uint8_t key);
    void leha_loh_print_key(uint8_t key);
};

#endif                                                                                                                                                      