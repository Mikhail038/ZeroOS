#include"irq.hpp"
#include"display.hpp"

uint32_t InterruptManager::handl_irq(uint8_t irq_number, uint32_t esp) {

    Cell* cell = (Cell*) 0xB8000;
    cell->set_char('Z');
    cell->set_bg_colour(red);

    return esp;
};

