#include "machine.h"

void set_verbosity(machine_t* machine, _Bool verbosity) {
    machine->is_verbose = verbosity;
}

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
    if(machine->is_verbose) {
        // long output
    } else {
        // short output
    }
}

void print_memory(machine_t* machine, uint32_t n, uint32_t m) {
    while(n <= m) {
        fprintf(stdout, "%04x\n", machine->general_memory[n]);
        ++n;
    }
}

void add_to_program_memory(machine_t* machine, char* line) {
    uint32_t i = 0;
    while(machine->program_memory[i] != NULL) {
        ++i;
    }
    machine->program_memory[i] = malloc(strlen(line)+1);
    strcpy(machine->program_memory[i], line);
    fprintf(stdout, "\"%s\" added to program memory at index %d\n", line, i);
}

uint32_t execute_program(machine_t* machine) {
    machine->pc = 0;
    printf("execute_program() begin\n");
    uint32_t err_counter = 0;
    uint32_t _i = 0;


    uint32_t i = 0;
    printf("PROGRAM MEMORY CONTENT:\n");
    while(machine->program_memory[i] != NULL) {
        printf("%s\n", machine->program_memory[i]);
        ++i;
    }

    while(_i <= 20) {
        ++_i;
        char* buf = malloc(50);
        //printf("%s\n", machine->program_memory[machine->pc]);
        strcpy(buf, machine->program_memory[machine->pc]);
        printf("EXECUTE_CODE BUF: %s , machine->pc = %d\n", buf, machine->pc);
        char* line_contents[MAX_LINE_ELEMENTS];

        uint8_t line_elem_counter = 0;

        _Bool is_unknown_instr = false; 
        
        // tokenize line
        char* tok = strtok(buf, delim);
        while(tok != NULL) {
            line_contents[line_elem_counter++] = tok;
            tok = strtok(NULL, delim);
        }

        // interpret tokenized form, assume there is no line with more than 5 words
        if(strcmp(line_contents[0], "mov") == 0) {
            uint8_t val;
             if(!strcmp(line_contents[2], "$ax")) {
                val = get_reg(machine, ax);
            } else if(!strcmp(line_contents[2], "$bx")) {
                val = get_reg(machine, bx);
            } else if(!strcmp(line_contents[2], "$cx")) {
                val = get_reg(machine, cx);
            } else if(!strcmp(line_contents[2], "$dx")) {
                val = get_reg(machine, dx);
            } else {
                val = atoi(line_contents[2]);
            }
            if(sizeof(val) != sizeof(uint32_t) && sizeof(val) != sizeof(uint8_t)) {
                fprintf(stderr, " [-](mov) invalid instruction argument! (operand)\n");
                ++err_counter;
            }
            // mov instr -> check for arguments
            if(strcmp(line_contents[1], "ax") == 0) {
                store_to_reg(machine, ax, val);
            } else if(strcmp(line_contents[1], "bx") == 0) {
                store_to_reg(machine, bx, val);
            } else if(strcmp(line_contents[1], "cx") == 0) {
                store_to_reg(machine, cx, val);
            } else if(strcmp(line_contents[1], "dx") == 0) {
                store_to_reg(machine, dx, val);

            } else if(line_contents[1][0] == '%') {
                // memory locations marked with %
                uint32_t memory_addr = atoi(line_contents[1] + 1);
                poke(machine, memory_addr, val);
            
            } else {
                fprintf(stderr, " [-] - invalid `mov` instruction arguments!\n");
                ++err_counter;
            }

        } else if(strcmp(line_contents[0], "cmp") == 0) {
            compare(machine, line_contents[1], line_contents[2]);
        } else if(strcmp(line_contents[0], "jmp") == 0) {
            jump(machine, atoi(line_contents[1]));
        } else if(strcmp(line_contents[0], "jz") == 0) {
            jump_if_zero(machine, atoi(line_contents[1]));
        } else if(strcmp(line_contents[0], "pop") == 0) {
            // first argument is the specified register
            if(strcmp(line_contents[1], "ax") == 0) {
                pop_stack(machine, ax);
            } else if(strcmp(line_contents[1], "bx") == 0) {
                pop_stack(machine, bx);
            } else if(strcmp(line_contents[1], "cx") == 0) {
                pop_stack(machine, cx);
            } else if(strcmp(line_contents[1], "dx") == 0) {
                pop_stack(machine, dx);
            }
        } else if(strcmp(line_contents[0], "push") == 0) {
            if(strcmp(line_contents[1], "ax") == 0) {
                push_stack(machine, ax);
            } else if(strcmp(line_contents[1], "bx") == 0) {
                push_stack(machine, bx);
            } else if(strcmp(line_contents[1], "cx") == 0) {
                push_stack(machine, cx);
            } else if(strcmp(line_contents[1], "dx") == 0) {
                push_stack(machine, dx);
            }
        } else if(strcmp(line_contents[0], "lea") == 0) {
            // TODO: implement
        } else if(strcmp(line_contents[0], "nop") == 0) {
            // NOTHING - most useful instruction ever
            // has to be included because of nop slides
            no_op(machine);
        } else if(strcmp(line_contents[0], "hlt") == 0) {
            halt(machine);
        } else if(strcmp(line_contents[0], "ret") == 0) {
            // TODO: return from function ?????
        } else if(strcmp(line_contents[0], "add") == 0) {
            uint8_t val;
                 // get val from get_reg
            if(!strcmp(line_contents[2], "$ax")) {
                val = get_reg(machine, ax);
            } else if(!strcmp(line_contents[2], "$bx")) {
                val = get_reg(machine, bx);
            } else if(!strcmp(line_contents[2], "$cx")) {
                val = get_reg(machine, cx);
            } else if(!strcmp(line_contents[2], "$dx")) {
                val = get_reg(machine, dx);
            } else {
                val = atoi(line_contents[2]);
            }
             
             if(sizeof(val) != sizeof(uint32_t) && sizeof(val) != sizeof(uint8_t)) {
                fprintf(stderr, " [-](add) invalid instruction argument! (operand)\n");
                ++err_counter;
            }
             if(!strcmp(line_contents[1], "ax")) {
                add_to_register(machine, ax, val);
             } else if(!strcmp(line_contents[1], "bx")) {
                add_to_register(machine, bx, val);
             } else if(!strcmp(line_contents[1], "cx")) {
                add_to_register(machine, cx, val);
             } else if(!strcmp(line_contents[1], "dx")) {
                add_to_register(machine, dx, val);
             } 
             else {
                 fprintf(stderr, " [-](add) invalid instruction argument! (register)\n");
                 ++err_counter;
             }
        } else if(strcmp(line_contents[0], "sub") == 0) {
            uint8_t val;
             if(!strcmp(line_contents[2], "$ax")) {
                val = get_reg(machine, ax);
            } else if(!strcmp(line_contents[2], "$bx")) {
                val = get_reg(machine, bx);
            } else if(!strcmp(line_contents[2], "$cx")) {
                val = get_reg(machine, cx);
            } else if(!strcmp(line_contents[2], "$dx")) {
                val = get_reg(machine, dx);
            } else {
                val = atoi(line_contents[2]);
            }
            if(sizeof(val) != sizeof(uint32_t) && sizeof(val) != sizeof(uint8_t)) {
                fprintf(stderr, " [-](sub) invalid instruction argument! (operand)\n");
                ++err_counter;
            }
             if(!strcmp(line_contents[1], "ax")) {
                sub_to_register(machine, ax, val);
             } else if(!strcmp(line_contents[1], "bx")) {
                sub_to_register(machine, bx, val);
             } else if(!strcmp(line_contents[1], "cx")) {
                sub_to_register(machine, cx, val);
             } else if(!strcmp(line_contents[1], "dx")) {
                sub_to_register(machine, dx, val);
             } 
             else {
                 fprintf(stderr, " [-](sub) invalid instruction argument! (register)\n");
                 ++err_counter;
             }
        } else if(strcmp(line_contents[0], "mul") == 0) {
            uint8_t val;
             if(!strcmp(line_contents[2], "$ax")) {
                val = get_reg(machine, ax);
            } else if(!strcmp(line_contents[2], "$bx")) {
                val = get_reg(machine, bx);
            } else if(!strcmp(line_contents[2], "$cx")) {
                val = get_reg(machine, cx);
            } else if(!strcmp(line_contents[2], "$dx")) {
                val = get_reg(machine, dx);
            } else {
                val = atoi(line_contents[2]);
            }
            if(sizeof(val) != sizeof(uint32_t) && sizeof(val) != sizeof(uint8_t)) {
                fprintf(stderr, " [-](mul) invalid instruction argument! (operand)\n");
                ++err_counter;
            }
             if(!strcmp(line_contents[1], "ax")) {
                mul_to_register(machine, ax, val);
             } else if(!strcmp(line_contents[1], "bx")) {
                mul_to_register(machine, bx, val);
             } else if(!strcmp(line_contents[1], "cx")) {
                mul_to_register(machine, cx, val);
             } else if(!strcmp(line_contents[1], "dx")) {
                mul_to_register(machine, dx, val);
             } 
             else {
                 fprintf(stderr, " [-](mul) invalid instruction argument! (register)\n");
                 ++err_counter;
             }
        } else if(strcmp(line_contents[0], "div") == 0) {
            uint8_t val;
             if(!strcmp(line_contents[2], "$ax")) {
                val = get_reg(machine, ax);
            } else if(!strcmp(line_contents[2], "$bx")) {
                val = get_reg(machine, bx);
            } else if(!strcmp(line_contents[2], "$cx")) {
                val = get_reg(machine, cx);
            } else if(!strcmp(line_contents[2], "$dx")) {
                val = get_reg(machine, dx);
            } else {
                val = atoi(line_contents[2]);
            }
            if(sizeof(val) != sizeof(uint32_t) && sizeof(val) != sizeof(uint8_t)) {
                fprintf(stderr, " [-](div) invalid instruction argument! (operand)\n");
                ++err_counter;
            }
             if(strcmp(line_contents[1], "ax")) {
                div_to_register(machine, ax, val);
             } else if(!strcmp(line_contents[1], "bx")) {
                div_to_register(machine, bx, val);
             } else if(!strcmp(line_contents[1], "cx")) {
                div_to_register(machine, cx, val);
             } else if(!strcmp(line_contents[1], "dx")) {
                div_to_register(machine, dx, val);
             } 
             else {
                 fprintf(stderr, " [-](div) invalid instruction argument! (register)\n");
                 ++err_counter;
             }
        } else {
            is_unknown_instr = true;
            
        }
        
        if(!is_unknown_instr) {
            printf("print_registers()\n");
            print_registers(machine);
        } else {
            fprintf(stderr, " [-] unknown instruction!\n");
            ++err_counter;
        }
        
        is_unknown_instr = false;
        
    
    }

    printf("execute() end\n");

    return err_counter;
}