#include"irq.hpp"
#include"display.hpp"

void putline(char* line)
{
    static unsigned short* video_memory_entry = (unsigned short*) 0xB8000;

    for (int cnt = 0; line[cnt] != '\0'; ++cnt)
    {
        video_memory_entry[cnt] = (video_memory_entry[cnt] & 0xFF00) | line[cnt]; //save high bytes (color info)
    }
}

InterruptManager::GateDescriptor InterruptManager::interrupt_descriptor_table[256];

InterruptManager* InterruptManager::active_interrupt_manager = 0;


void InterruptManager::set_interrupt_descriptor_table_entry(
    uint8_t interrupt_number,
    uint16_t code_segment_selector_offset,
    void (*handler)(),
    uint8_t descriptor_privelege_level,
    uint8_t descriptor_type)
{
    const uint8_t IDT_DESC_PRESENT = 0x80;

    interrupt_descriptor_table[interrupt_number].handle_address_low_bits = ((uint32_t) handler) & 0xFFFF;
    interrupt_descriptor_table[interrupt_number].handle_address_high_bits = (((uint32_t) handler) >> 16) & 0xFFFF;    

    interrupt_descriptor_table[interrupt_number].gdt_code_segment_selector = code_segment_selector_offset;

    interrupt_descriptor_table[interrupt_number].access = IDT_DESC_PRESENT | ((descriptor_privelege_level & 3) << 5) | descriptor_type ;

    interrupt_descriptor_table[interrupt_number].reserved = 0;
}

InterruptManager::InterruptManager(GlobalDescriptorTable* gdt) :
    pic_master_command(0x20),
    pic_master_data(0x21),
    pic_slave_command(0xA0),
    pic_slave_data(0xA1)
{
    uint16_t code_segment = gdt->get_code_segment_selector();
    
    const uint8_t IDT_INTERRUPT_GATE = 0xE;

    for (uint16_t cnt = 0; cnt != 256; ++cnt)
    {
        handlers[cnt] = 0;
        set_interrupt_descriptor_table_entry(cnt, code_segment, &asm_ignore_irq_, 0, IDT_INTERRUPT_GATE);
    }

    set_interrupt_descriptor_table_entry(hardware_offset + 0x00, 
                      code_segment, &asm_handle_irq_0x00, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x01, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x02, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x03, 
                      code_segment, &asm_handle_irq_0x00, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x04, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x05, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x06, 
                      code_segment, &asm_handle_irq_0x00, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x07, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x08, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x09, 
                      code_segment, &asm_handle_irq_0x00, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x0A, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x0B, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x0C, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x0D, 
                      code_segment, &asm_handle_irq_0x00, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x0E, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);
    set_interrupt_descriptor_table_entry(hardware_offset + 0x0F, 
                      code_segment, &asm_handle_irq_0x01, 0, IDT_INTERRUPT_GATE);

    pic_master_command.write(0x11);
    pic_slave_command.write(0x11);

    pic_master_data.write(0x20);
    pic_slave_data.write(0x28);

    pic_master_data.write(0x04);
    pic_slave_data.write(0x02);

    pic_master_data.write(0x01);
    pic_slave_data.write(0x01);

    pic_master_data.write(0x00);
    pic_slave_data.write(0x00);

    InterruptDescriptorTablePointer idtp;

    idtp.size = 256 * sizeof(GateDescriptor) - 1;
    idtp.base = (uint32_t) interrupt_descriptor_table;

    asm volatile("lidt %0" : : "m" (idtp));
}

void InterruptManager::activate()
{
    if (active_interrupt_manager != 0)
    {
        active_interrupt_manager->deactivate();
    }
    active_interrupt_manager = this;

    // {
        asm("sti");
    // }
}

void InterruptManager::deactivate()
{
    if (active_interrupt_manager == this)
    {
        active_interrupt_manager = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::handle_irq(uint8_t irq_number, uint32_t esp) 
{
    if (active_interrupt_manager != 0)
    {
       return active_interrupt_manager->do_handle_irq(irq_number, esp);
    }
    return esp;
};

uint32_t InterruptManager::do_handle_irq(uint8_t irq_number, uint32_t esp) 
{
    if (handlers[irq_number] != 0)
    {
        esp = handlers[irq_number]->handle_interrupt(esp);
    }
    else if (irq_number != hardware_offset + 0x00)
    {
        char* foo = "UNHANDLED INTERRUPT 0x00";
        char* hex = "0123456789ABCDEF";

        foo[22] = hex[(irq_number >> 4) & 0x0F];
        foo[23] = hex[irq_number & 0x0F];

        putline(foo);
    }

    if ((irq_number >= hardware_offset) && (irq_number < (hardware_offset + 0x10)))
    {
        pic_master_command.write(0x20);

        if (irq_number >= hardware_offset + 0x08)
        {
            pic_slave_command.write(0x20);
        }
    }

    return esp;
}

InterruptHandler::InterruptHandler(uint8_t irq_number_, InterruptManager* interrupt_manager_) :
    irq_number(irq_number_),
    interrupt_manager(interrupt_manager_)
{
    interrupt_manager->handlers[irq_number] = this;
}

InterruptHandler::~InterruptHandler()
{
    if (interrupt_manager->handlers[irq_number] == this)
    {
        interrupt_manager->handlers[irq_number] = 0;
    }
}

uint32_t InterruptHandler::handle_interrupt(uint32_t esp)
{
    return esp;
}
