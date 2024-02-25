
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
    