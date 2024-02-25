#ifndef IRQ_HPP
#define IRQ_HPP

#include "types.hpp"
#include "port.hpp"
#include "port_slow.hpp"

#include "gdt.hpp"


class InterruptManager
{
protected:
    struct GateDescriptor
    {
        uint16_t    handle_address_low_bits;
        uint16_t    gdt_code_segment_selector;
        uint8_t     reserved;
        uint8_t     access;
        uint16_t    handle_address_high_bits;

    } __attribute__((packed));
    
static GateDescriptor interrupt_descriptor_table[256];

struct InterruptDescriptorTablePointer
{
    uint16_t size;
    uint32_t base;
}__attribute__ ((packed));

static void set_interrupt_descriptor_table_entry(
    uint8_t irq_number,
    uint16_t code_segment_selector_offset,
    void (*handler)(),
    uint8_t descriptor_privelege_level,
    uint8_t descriptor_type
);

    PortSlow<uint8_t> pic_master_command;
    PortSlow<uint8_t> pic_master_data;
    PortSlow<uint8_t> pic_slave_command;
    PortSlow<uint8_t> pic_slave_data;


public:
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager() = default;

    void activate();

    static uint32_t handle_irq(uint8_t irq_number, uint32_t esp);
    
    static void asm_ignore_irq_();

    static void asm_handle_irq_0x00();
    static void asm_handle_irq_0x01();

    static void asm_handle_exc_0x00();
    static void asm_handle_exc_0x01();
};


#endif