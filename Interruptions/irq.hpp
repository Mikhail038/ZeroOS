#ifndef IRQ_HPP
#define IRQ_HPP

#include "types.hpp"
#include "port.hpp"
#include "port_slow.hpp"

#include "gdt.hpp"

class InterruptHandler;

class InterruptManager final
{
protected:
    friend class InterruptHandler; 

    static InterruptManager* active_interrupt_manager;

    InterruptHandler* handlers[BYTE];

    struct GateDescriptor final
    {
        uint16_t    handle_address_low_bits;
        uint16_t    gdt_code_segment_selector;
        uint8_t     reserved;
        uint8_t     access;
        uint16_t    handle_address_high_bits;

    } __attribute__((packed));
    
    static GateDescriptor interrupt_descriptor_table[BYTE];

    struct InterruptDescriptorTablePointer final
    {
        uint16_t size;
        uint32_t base;
    }__attribute__ ((packed));

    static void set_interrupt_descriptor_table_entry(
        uint8_t irq_number,
        uint16_t code_segment_selector_offset,
        void (*handler)(),
        uint8_t descriptor_privelege_level,
        uint8_t descriptor_type);

    PortSlow<uint8_t> pic_master_command;
    PortSlow<uint8_t> pic_master_data;
    PortSlow<uint8_t> pic_slave_command;
    PortSlow<uint8_t> pic_slave_data;

    const uint16_t hardware_offset = 0x20; 

public:
    InterruptManager(GlobalDescriptorTable* gdt);
    ~InterruptManager() = default;

    void activate();
    void deactivate();

    static uint32_t handle_irq(uint8_t irq_number, uint32_t esp);
    uint32_t do_handle_irq(uint8_t irq_number, uint32_t esp) const;
    

    static void asm_ignore_irq_();

    static void asm_handle_irq_0x00();
    static void asm_handle_irq_0x01();
    static void asm_handle_irq_0x02();
    static void asm_handle_irq_0x03();
    static void asm_handle_irq_0x04();
    static void asm_handle_irq_0x05();
    static void asm_handle_irq_0x06();
    static void asm_handle_irq_0x07();
    static void asm_handle_irq_0x08();
    static void asm_handle_irq_0x09();
    static void asm_handle_irq_0x0A();
    static void asm_handle_irq_0x0B();
    static void asm_handle_irq_0x0C();
    static void asm_handle_irq_0x0D();
    static void asm_handle_irq_0x0E();
    static void asm_handle_irq_0x0F();

    static void asm_handle_exc_0x00();
    static void asm_handle_exc_0x01();
};

class InterruptHandler
{
protected:
    uint8_t irq_number;
    InterruptManager* interrupt_manager;

    InterruptHandler(uint8_t irq_number_, InterruptManager* interrupt_manager_);
    ~InterruptHandler();
public:
    virtual uint32_t handle_interrupt(uint32_t esp);
};

#endif