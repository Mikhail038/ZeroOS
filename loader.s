.set MAGIC, 0x1BADB002 
.set FLAGS, (1<<0 | 1<<1)
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
	.align 4
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.extern kernel_main
.global loader

loader: 
    mov $kernel_stack, %esp
    push %eax
    push %ebx
    call kernel_main

_stop:
    cli
    hlt
    jmp _stop


.section .bss
.align 16
.space 4096; #2MiB
kernel_stack:
