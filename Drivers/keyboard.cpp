#include "keyboard.hpp"

KeyboardDriver::KeyboardDriver(InterruptManager* interrupt_manager_) :
    InterruptHandler(keyboard_irq, interrupt_manager_),
    data_port(0x60),
    command_port(0x64)
{
    while (command_port.read() & 0x01)
    {
        data_port.read();
    }

    command_port.write(0xAE); //activate
    command_port.write(0x20); //get current state

    uint8_t status = (data_port.read() | 1) & ~0x10;

    command_port.write(0x60); //set state
    data_port.write(status);

    data_port.write(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{

}

uint32_t KeyboardDriver::handle_interrupt(uint32_t esp)
{
    uint8_t key = data_port.read();

    return esp;
}