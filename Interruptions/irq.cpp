#include"irq.hpp"
#include"display.hpp"


InterruptManager::GateDescriptor InterruptManager::interrupt_descriptor_table[256];


void InterruptManager::set_interrupt_descriptor_table_entry(
    uint8_t interrupt_number,
    uint16_t code_segment_selector_offset,
    void (*handler)(),
    uint8_t descriptor_privelege_level,
    uint8_t descriptor_type
)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    interrupt_descriptor_table[interrupt_number].handle_address_high_bits = ((uint32_t) handler) & 0xFFFF;
    interrupt_descriptor_table[interrupt_number].handle_address_high_bits = (((uint32_t) handler) >> 16) & 0xFFFF;    

    interrupt_descriptor_table[interrupt_number].gdt_code_segment_selector = code_segment_selector_offset;

    interrupt_descriptor_table[interrupt_number].access = IDT_DESC_PRESENT | descriptor_type | ((descriptor_privelege_level & 3) << 5);

    interrupt_descriptor_table[interrupt_number].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt)
{
    const uint16_t IRQ_BASE = 0x20;

    uint16_t code_segment = gdt->get_code_segment_selector();
    
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t cnt = 0; cnt != 256; ++cnt)
    {
        set_interrupt_descriptor_table_entry(cnt, code_segment, &asm_ignore_irq_, 0, IDT_INTERRUPT_GATE);
    }

    set_interrupt_descriptor_table_entry(IRQ_BASE + 0x00, 
                      code_segment, &asm_handle_irq_0x00, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(IRQ_BASE + 0x01, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);

    InterruptDescriptorTablePointer idtp;

    idtp.size = 256 * sizeof(GateDescriptor) - 1;
    idtp.base = (uint32_t) interrupt_descriptor_table;

    asm volatile("lidt %0" : : "m" (idtp));
}

void InterruptManager::activate()
{

}

uint32_t InterruptManager::handle_irq(uint8_t irq_number, uint32_t esp) {

    Cell* cell = (Cell*) 0xB8000;
    cell->set_char('Z');
    cell->set_bg_colour(red);

    return esp;
};