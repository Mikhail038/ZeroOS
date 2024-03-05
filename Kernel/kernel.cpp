#include "types.hpp"
#include "gdt.hpp"
#include "display.hpp"
#include "irq.hpp"
#include "keyboard.hpp"
#include "mouse.hpp"
#include "driver.hpp"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void call_constructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; ++i)
    {
        (*i)();
    }
}

#define PRINT_DONE display.print_line("DONE\n")


extern "C" void kernel_main(void* multiboot_struture, uint32_t magic_number)
{
    Display display({' ', black, white});

    display.print_line("DISPLAY INIT:");
    PRINT_DONE;

    display.print_line("IRQ TABLE INIT:");

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(&gdt);
    PRINT_DONE;

    display.print_line("DRIVERS INIT:");
    DriverManager driver_mgr;

    KeyboardDriver keyboard(&interrupts, display);
    // MouseDriver mouse(&interrupts, display);

    driver_mgr.add_new_driver(&keyboard);
    PRINT_DONE;


    display.print_line("DRIVERS ACTIVATION:");
    driver_mgr.activae_all();

    interrupts.activate();
    PRINT_DONE;



    while (true);
}

#undef PRINT_DONE