#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"
#include "timer.h"
#include "ports.h"
#include <stdint.h>

isr_t interrupt_handlers[256];

/* Can't do this with a loop because we need the address
 * of the function names */
void isr_install() {
    register_int_handler(0, isr0);
    register_int_handler(1, isr1);
    register_int_handler(2, isr2);
    register_int_handler(3, isr3);
    register_int_handler(4, isr4);
    register_int_handler(5, isr5);
    register_int_handler(6, isr6);
    register_int_handler(7, isr7);
    register_int_handler(8, isr8);
    register_int_handler(9, isr9);
    register_int_handler(10, isr10);
    register_int_handler(11, isr11);
    register_int_handler(12, isr12);
    register_int_handler(13, isr13);
    register_int_handler(14, isr14);
    register_int_handler(15, isr15);
    register_int_handler(16, isr16);
    register_int_handler(17, isr17);
    register_int_handler(18, isr18);
    register_int_handler(19, isr19);
    register_int_handler(20, isr20);
    register_int_handler(21, isr21);
    register_int_handler(22, isr22);
    register_int_handler(23, isr23);
    register_int_handler(24, isr24);
    register_int_handler(25, isr25);
    register_int_handler(26, isr26);
    register_int_handler(27, isr27);
    register_int_handler(28, isr28);
    register_int_handler(29, isr29);
    register_int_handler(30, isr30);
    register_int_handler(31, isr31);

    // Remap the PIC
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0); 

    // Install the IRQs
    register_int_handler(32, irq0);
    register_int_handler(33, irq1);
    register_int_handler(34, irq2);
    register_int_handler(35, irq3);
    register_int_handler(36, irq4);
    register_int_handler(37, irq5);
    register_int_handler(38, irq6);
    register_int_handler(39, irq7);
    register_int_handler(40, irq8);
    register_int_handler(41, irq9);
    register_int_handler(42, irq10);
    register_int_handler(43, irq11);
    register_int_handler(44, irq12);
    register_int_handler(45, irq13);
    register_int_handler(46, irq14);
    register_int_handler(47, irq15);

    set_idt(); // Load with ASM
}

/* To print the message which defines every exception */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t r) {
    kprint("received interrupt: ");
    char s[3];
    int_to_ascii(r.int_no, s);
    kprint(s);
    kprint("\n");
    kprint(exception_messages[r.int_no]);
    kprint("\n");
}

void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r.int_no >= 40) port_byte_out(0xA0, 0x20); /* slave */
    port_byte_out(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

void irq_install() {
    /* Enable interruptions */
    asm volatile("sti");
    /* IRQ0: timer */
    init_timer(50);
    /* IRQ1: keyboard */
    init_keyboard();
}