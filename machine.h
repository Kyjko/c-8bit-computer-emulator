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
#include <stdlib.h>

// define ansi colors if compiled with unix system
#ifdef __unix__
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#endif

const char delim[2] = " ";

#define GEN_MEM_CAPACITY 1024*64
#define PROGRAM_MEM_CAPACITY 1024
#define STACK_CAPACITY 1024
#define RES_X 24
#define RES_Y 24
// assume there's a maximum of 50 space-delimetered "words" in a line ( applies to comments as well!)
#define MAX_LINE_ELEMENTS 50

typedef struct machine {
    uint8_t ax, bx, cx, dx, sp, bp, pc, fl;

    struct mem {
        uint8_t general_memory[GEN_MEM_CAPACITY];
        uint8_t stack[STACK_CAPACITY];
    };

    uint8_t screen_output[RES_X*RES_Y];
    char* program_memory[PROGRAM_MEM_CAPACITY];

    _Bool is_verbose;

} machine_t;

typedef enum REGS {
    ax, bx, cx, dx, sp, bp, pc, fl
} REGS;

void set_verbosity(machine_t* machine, _Bool verbosity);
uint32_t execute_program(machine_t* machine);
void add_to_program_memory(machine_t* machine, char* line);
void reset(machine_t* machine);
void store_to_reg(machine_t* machine, enum REGS reg, uint8_t value);
uint8_t get_reg(machine_t* machine, enum REGS reg);
void poke(machine_t* machine, uint32_t addr, uint8_t value);
uint8_t peek(const machine_t* machine, uint32_t addr);
void poke_stack(machine_t* machine, uint32_t addr, uint8_t value);
uint8_t peek_stack(const machine_t* machine, uint32_t addr);
void compare(machine_t* machine, char* reg_1, char* reg_2);
uint32_t stack_bottom(machine_t* machine);
void pop_stack(machine_t* machine, enum REGS reg);
void push_stack(machine_t* machine, enum REGS reg);
void halt(machine_t* machine);
void print_registers(machine_t* machine);
void print_memory(machine_t* machine, uint32_t n, uint32_t m);
void print_register(machine_t* machine, enum REGS reg);
void add_to_register(machine_t* machine, enum REGS reg, uint8_t value);
void sub_to_register(machine_t* machine, enum REGS reg, uint8_t value);
void mul_to_register(machine_t* machine, enum REGS reg, uint8_t value);
void div_to_register(machine_t* machine, enum REGS reg, uint8_t value);
uint32_t jump(machine_t* machine, uint32_t addr);
uint32_t jump_if_zero(machine_t* machine, uint32_t addr);
void no_op(machine_t* machine);
void show_screen_output(machine_t* machine);



#endif