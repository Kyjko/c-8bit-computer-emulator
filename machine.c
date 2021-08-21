#include "machine.h"

void reset(machine_t* machine) {
    memset(machine->general_memory, 0, sizeof(machine->general_memory));
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
        case ax: machine->ax = value; break;
        case bx: machine->bx = value; break;
        case cx: machine->cx = value; break;
        case dx: machine->dx = value; break;
        case sp: machine->sp = value; break;
        case bp: machine->bp = value; break;
        case pc: machine->pc = value; break;
        case fl: machine->fl = value; break;
        default: fprintf(stderr, "[-] store_to_reg() : invalid register identifier\n"); return;
    }

    machine->pc++;
}

void add_to_register(machine_t* machine, enum REGS reg, uint8_t value) {
    switch(reg) {
        case ax: machine->ax += value; break;
        case bx: machine->bx += value; break;
        case cx: machine->cx += value; break;
        case dx: machine->dx += value; break;
        default: fprintf(stderr, "[-] add_to_register() : invalid register identifier\n"); return;
    }

    machine->pc++;
}

void sub_to_register(machine_t* machine, enum REGS reg, uint8_t value) {
    switch(reg) {
        case ax: machine->ax -= value; break;
        case bx: machine->bx -= value; break;
        case cx: machine->cx -= value; break;
        case dx: machine->dx -= value; break;
        default: fprintf(stderr, "[-] sub_to_register() : invalid register identifier\n"); return;
    }
    
    machine->pc++;
}

void mul_to_register(machine_t* machine, enum REGS reg, uint8_t value) {
    switch(reg) {
        case ax: machine->ax *= value; break;
        case bx: machine->bx *= value; break;
        case cx: machine->cx *= value; break;
        case dx: machine->dx *= value; break;
        default: fprintf(stderr, "[-] mul_to_register() : invalid register identifier\n"); return;
    }
    
    machine->pc++;
}

void div_to_register(machine_t* machine, enum REGS reg, uint8_t value) {
    switch(reg) {
        case ax: machine->ax /= value; break;
        case bx: machine->bx /= value; break;
        case cx: machine->cx /= value; break;
        case dx: machine->dx /= value; break;
        default: fprintf(stderr, "[-] div_to_register() : invalid register identifier\n"); return;
    }
    
    machine->pc++;
}

uint32_t jump(machine_t* machine, uint32_t addr) {
    machine->pc = addr;
    return machine->pc;
}

uint32_t jump_if_zero(machine_t* machine, uint32_t addr) {
    if(machine->fl != 0) {
        machine->pc = addr;
    }

    return machine->pc;
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

uint32_t get_stack_bottom(machine_t* machine) {
    uint32_t i = 0;
    while(machine->stack[i] != 0) {
        ++i;
    }
    machine->sp = i + 1;
    return i + 1;
}

void pop_stack(machine_t* machine, enum REGS reg) {
    switch(reg) {
        case ax: {
            machine->ax = machine->stack[get_stack_bottom(machine)];
            machine->stack[get_stack_bottom(machine)] = 0;
        }
        case bx: {
            machine->bx = machine->stack[get_stack_bottom(machine)];
            machine->stack[get_stack_bottom(machine)] = 0;
        }
        case cx: {
            machine->cx = machine->stack[get_stack_bottom(machine)];
            machine->stack[get_stack_bottom(machine)] = 0;
        }
        case dx: {
            machine->dx = machine->stack[get_stack_bottom(machine)];
            machine->stack[get_stack_bottom(machine)] = 0;
        }
        default: {
            fprintf(stderr, "[-] pop() : invalid register argument!\n");
        }
    }

    get_stack_bottom(machine);
    machine->pc++;
}
void push_stack(machine_t* machine, enum REGS reg) {
    uint32_t stack_bottom = get_stack_bottom(machine);
    switch(reg) {
        case ax: {
            machine->stack[stack_bottom] = machine->ax;
        }
        case bx: {
            machine->stack[stack_bottom] = machine->bx;
        }
        case cx: {
            machine->stack[stack_bottom] = machine->cx;
        }
        case dx: {
            machine->stack[stack_bottom] = machine->dx;
        }
        default: {
            fprintf(stderr, "[-] push() : invalid register argument!\n");
        }
    }

    get_stack_bottom(machine);
    machine->pc++;
}

uint8_t peek_stack(const machine_t* machine, uint32_t addr) {
    if(addr < 0 || addr >= STACK_CAPACITY) {
        fprintf(stderr, "[-] peek_stack() : invalid memory address\n");
        return 0;
    }

    return machine->stack[addr];
}

void compare(machine_t* machine, char* reg_1, char* reg_2) {
    if((strcmp(reg_1, "ax") == 0 && strcmp(reg_2, "bx") == 0) || (strcmp(reg_1, "bx") == 0 && strcmp(reg_2, "ax") == 0)) {
        if(machine->ax == machine->bx) {
            machine->fl = 1;
        }
    }

    if((strcmp(reg_1, "ax") == 0 && strcmp(reg_2, "cx") == 0) || (strcmp(reg_1, "cx") == 0 && strcmp(reg_2, "ax") == 0)) {
        if(machine->ax == machine->cx) {
            machine->fl = 1;
        }
    }

    if((strcmp(reg_1, "ax") == 0 && strcmp(reg_2, "dx") == 0) || (strcmp(reg_1, "dx") == 0 && strcmp(reg_2, "ax") == 0)) {
        if(machine->ax == machine->dx) {
            machine->fl = 1;
        }
    }

    if((strcmp(reg_1, "bx") == 0 && strcmp(reg_2, "cx") == 0) || (strcmp(reg_1, "cx") == 0 && strcmp(reg_2, "bx") == 0)) {
        if(machine->bx == machine->cx) {
            machine->fl = 1;
        }
    }

    if((strcmp(reg_1, "cx") == 0 && strcmp(reg_2, "dx") == 0) || (strcmp(reg_1, "dx") == 0 && strcmp(reg_2, "cx") == 0)) {
        if(machine->cx == machine->dx) {
            machine->fl = 1;
        }
    }

    if((strcmp(reg_1, "bx") == 0 && strcmp(reg_2, "dx") == 0) || (strcmp(reg_1, "dx") == 0 && strcmp(reg_2, "bx") == 0)) {
        if(machine->bx == machine->dx) {
            machine->fl = 1;
        }
    }

    machine->pc++;
}

void halt(machine_t* machine) {
    // TODO: halt
    machine->pc++;
}

void print_registers(machine_t* machine) {
    fprintf(stdout, "ax:\t%04x\nbx:\t%04x\ncx:\t%04x\ndx:\t%04x\nsp:\t%04x\nbp:\t%04x\npc:\t%04x\nfl:\t%04x\n", 
    machine->ax, machine->bx, machine->cx, machine->dx, machine->sp, machine->bp, machine->pc, machine->fl);
}  
void print_register(machine_t* machine, enum REGS reg) {
    switch(reg) {
        case ax: fprintf(stdout, "ax:\t%04x\n", machine->ax); return;
        case bx: fprintf(stdout, "bx:\t%04x\n", machine->bx); return;
        case cx: fprintf(stdout, "cx:\t%04x\n", machine->cx); return;
        case dx: fprintf(stdout, "dx:\t%04x\n", machine->dx); return;
        case sp: fprintf(stdout, "sp:\t%04x\n", machine->sp); return;
        case bp: fprintf(stdout, "bp:\t%04x\n", machine->bp); return;
        case pc: fprintf(stdout, "pc:\t%04x\n", machine->pc); return;
        case fl: fprintf(stdout, "fl:\t%04x\n", machine->fl); return;
        default: {}
    }
}

uint8_t get_reg(machine_t* machine, enum REGS reg) {
    switch(reg) {
        case ax: return machine->ax;
        case bx: return machine->bx;
        case cx: return machine->cx;
        case dx: return machine->dx;
        case sp: return machine->sp;
        case bp: return machine->bp;
        case pc: return machine->pc;
        case fl: return machine->fl;
        default: fprintf(stderr, "[-] get_reg() : invalid register identifier\n");
    }

    return 0;
}

void no_op(machine_t* machine) {
    return;
}

void show_screen_output(machine_t* machine) {
    // TODO
}

void print_memory(machine_t* machine, uint32_t n, uint32_t m) {
    while(n <= m) {
        fprintf(stdout, "%04x\n", machine->general_memory[n]);
        ++n;
    }
}