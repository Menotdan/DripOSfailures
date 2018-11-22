#include "idt.h"

int register_interrupt_handler(size_t num, void (*handler)(void), uint8_t ist) {
    uint64_t p = (uint64_t)handler;

    idt[num].low = (p & 0xffff);
    idt[num].sel = KERNEL_CS;
    idt[num].ist = ist;
    idt[num].type_attr = 0x8e;
    idt[num].mid = ((p & 0xffff0000) >> 16);
    idt[num].high = ((p & 0xffffffff00000000) >> 32);
    idt[num].always0 = 0;

    return 0;
}

void start_idt() {
    for (size_t inter = 0; inter < 256; inter++) {
        register_interrupt_handler(inter, int_handler, 0, 0x8e);
    }
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidt (%0)" : : "m" (&idt_reg));
}
