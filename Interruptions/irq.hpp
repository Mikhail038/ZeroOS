
#ifndef IRQ_HPP
#define IRQ_HPP

#include"types.hpp"
#include"port.hpp"

    class InterruptManager
    {
        
    public:
        static uint32_t handl_irq(uint8_t irq_number, uint32_t esp);
    };


#endif