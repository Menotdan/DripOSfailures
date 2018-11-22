#ifndef IDT_H
#define IDT_H

#include "types.h"
#include <stdint.h>
#include <stddef.h>

/* Segment selectors */
#define KERNEL_CS 0x08

/* How every interrupt gate (handler) is defined */
typedef struct {
    uint16_t low; // offset bits 0..15
    uint16_t sel; // a code segment selector in GDT or LDT
    uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t type_attr; // type and attributes
    uint16_t mid; // offset bits 16..31
    uint32_t high; // offset bits 32..63
    uint32_t always0;     // reserved
} __attribute__((packed)) idt_gate_t ;

/* A pointer to the array of interrupt handlers.
 * Assembly instruction 'lidt' will read it */
typedef struct {
    u16 limit;
    u32 base;
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;


/* Functions implemented in idt.c */
void set_idt_gate(int n, u32 handler);
void set_idt();

#endif