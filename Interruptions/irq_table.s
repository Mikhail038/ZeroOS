
.set IRQ_BASE, 0x20

.section .text

.extern  _ZN16InterruptManager10handle_irqEhj

.global _ZN16InterruptManager15asm_ignore_irq_Ev

.macro asm_handle_irq number
.global _ZN16InterruptManager19asm_handle_irq_\number\()Ev
_ZN16InterruptManager19asm_handle_irq_\number\()Ev:
    movb $\number, (irq_number)
    jmp irq_bottom
.endm

.macro asm_handle_exception number
.global _ZN16InterruptManager26asm_handle_exception\number\()Ev
_ZN16InterruptManager26asm_handle_exception\number\()Ev:
    movb $\number + IRQ_BASE, (irq_number)
    jmp irq_bottom
.endm


asm_handle_irq 0x00
asm_handle_irq 0x01

irq_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (irq_number)
    call _ZN16InterruptManager10handle_irqEhj
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

_ZN16InterruptManager15asm_ignore_irq_Ev:

    iret

.data
    irq_number: .byte 0
    