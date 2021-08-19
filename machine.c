#include "machine.h"

void reset(machine_t* machine) {
    memset(machine->general_memory, 0, sizeoof(machine->general_memory));
    memset(machine->stack, 0, sizeof(machine->stack));
    machine->ax = 0;
    machine->bx = 0;
    machine->cx = 0;
    machine->dx = 0;
    machine->sp = 0;
    machine->bp = 0;
    machine->pc = 0;
    machine->fl = 0;
}

void store_to_reg(machine_t* machine, REGS reg, uint8_t value) {
    switch(reg) {
        case ax: machine->ax = value;
        case bx: machine->bx = value;
        case cx: machine->cx = value;
        case dx: machine->dx = value;
        case sp: machine->sp = value;
        case bp: machine->bp = value;
        case pc: machine->pc = value;
        case fl: machine->fl = value;
        default: fprintf(stderr, "[-] store_to_reg() : invalid register identifier\n"); return;
    }
}

void poke(machine_t* machine, uint32_t addr, uint8_t value) {
    if(addr < 0 || addr >= GEN_MEM_CAPACITY) {
        fprintf(stderr, "[-] poke() : invalid memory address\n");
    }
    machine->general_memory[addr] = value;
}

uint8_t peek(const machine_t* machine, uint32_t addr) {
    if(addr < 0 || addr >= GEN_MEM_CAPACITY) {
        fprintf(stderr, "[-] peek() : invalid memory address\n");
        return 0;
    }

    return machine->general_memory[addr];
}

void poke_stack(machine_t* machine, uint32_t addr, uint8_t value) {
    if(addr < 0 || addr >= STACK_CAPACITY) {
        fprintf(stderr, "[-] poke_stack() : invalid memory address\n");
    }
    machine->stack[addr] = value;
}

uint8_t peek_stack(const machine_t* machine, uint32_t addr) {
    if(addr < 0 || addr >= STACK_CAPACITY) {
        fprintf(stderr, "[-] peek_stack() : invalid memory address\n");
        return 0;
    }

    return machine->stack[addr];
}