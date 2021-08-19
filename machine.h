/*

    Registers: 
        ax, bx, cx, dx, sp, bp, pc, fl

*/

#ifndef MACHINE_H_
#define MACHINE_H_

#include <memory.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define GEN_MEM_CAPACITY 1024*64
#define STACK_CAPACITY 1024
#define RES_X 24
#define RES_Y 24

typedef struct monitor {
    uint8_t pixels[RES_X*RES_Y][3];


} monitor_t;

typedef struct machine {
    uint8_t ax, bx, cx, dx, sp, bp, pc, fl;

    struct mem {
        uint8_t general_memory[GEN_MEM_CAPACITY];
        uint8_t stack[STACK_CAPACITY];
    };

    monitor_t monitor;

} machine_t;

typedef enum REGS {
    ax, bx, cx, dx, sp, bp, pc, fl
} REGS;


void reset(machine_t* machine);
void store_to_reg(machine_t* machine, enum REGS reg, uint8_t value);
void poke(machine_t* machine, uint32_t addr, uint8_t value);
uint8_t peek(const machine_t* machine, uint32_t addr);
void poke_stack(machine_t* machine, uint32_t addr, uint8_t value);
uint8_t peek_stack(const machine_t* machine, uint32_t addr);
void compare(machine_t* machine, char* reg_1, char* reg_2);
uint32_t stack_bottom(machine_t* machine);
void pop_stack(machine_t* machine, enum REGS reg);
void push_stack(machine_t* machine, enum REGS reg);
void halt(machine_t* machine);

#endif