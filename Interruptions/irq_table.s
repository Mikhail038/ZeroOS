
.set IRQ_BASE, 0x20

.section .text

.extern  _ZN16InterruptManager10handle_irqEhj

.macro Handle_exc number
.global _ZN16InterruptManager19asm_handle_exc_\number\()Ev
_ZN16InterruptManager19asm_handle_exc_\number\()Ev:
    movb $\number, (irq_number)
    jmp irq_bottom
.endm

.macro Handle_irq number
.global _ZN16InterruptManager19asm_handle_irq_\number\()Ev
_ZN16InterruptManager19asm_handle_irq_\number\()Ev:
    movb $\number + IRQ_BASE, (irq_number)
    jmp irq_bottom
.endm

Handle_exc 0x00
Handle_exc 0x01

Handle_irq 0x00
Handle_irq 0x01
Handle_irq 0x02
Handle_irq 0x03
Handle_irq 0x04
Handle_irq 0x05
Handle_irq 0x06
Handle_irq 0x07
Handle_irq 0x08
Handle_irq 0x09
Handle_irq 0x0A
Handle_irq 0x0B
Handle_irq 0x0C
Handle_irq 0x0D
Handle_irq 0x0E
Handle_irq 0x0F


irq_bottom:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (irq_number)
    call _ZN16InterruptManager10handle_irqEhj
    add %esp, 6
    mov %eax, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa


.global _ZN16InterruptManager15asm_ignore_irq_Ev

_ZN16InterruptManager15asm_ignore_irq_Ev:

    iret

.data
    irq_number: .byte 0
    