/*

    Parse custom assembly code

*/

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "machine.h"

// assume one line doesn't contain more than 8 bits worth of chars
#define BUF_LEN 2<<9-1
// assume there's a maximum of 5 space-delimetered "words" in a line
#define MAX_LINE_ELEMENTS 5

typedef enum INSTR {
    mov, cmp, jmp, jz, pop, push, lea, nop, hlt, ret, add, sub, mul, div
} INSTR;

const char delim[2] = " ";


int32_t read_code(machine_t* machine) {
    
    // custom assembly code parsing here
    FILE* fp = fopen("./code.kasm", "r");
    if(fp == NULL) {
        fprintf(stderr, "[-] fopen() - Cannot find the source file!\n");
        return -1;
    }

    uint32_t err_counter = 0;
    
    // line = buf
    char buf[BUF_LEN];
    uint32_t ln = 0;
    while(fgets(buf, BUF_LEN, fp)) {
        fprintf(stdout, "LINE #%d: %s\n", ++ln);

        char* line_contents[MAX_LINE_ELEMENTS];

        uint8_t line_elem_counter = 0;
        
        // tokenize line
        char* tok = strtok(buf, delim);
        while(tok != NULL) {
            line_contents[line_elem_counter++] = tok;
            tok = strtok(NULL, delim);
        }

        // interpret tokenized form, assume there is no line with more than 5 words
        if(strcmp(line_contents[0], "mov") == 0) {
            // mov instr -> check for arguments
            if(strcmp(line_contents[1], "ax") == 0) {
                store_to_reg(machine, ax, line_contents[2]);
            } else if(strcmp(line_contents[1], "bx") == 0) {
                store_to_reg(machine, bx, line_contents[2]);
            } else if(strcmp(line_contents[1], "cx") == 0) {
                store_to_reg(machine, cx, line_contents[2]);
            } else if(strcmp(line_contents[1], "dx") == 0) {
                store_to_reg(machine, dx, line_contents[2]);
            } else {
                fprintf(stderr, " [COMPILATION] - invalid `mov` instruction arguments!\n");
                ++err_counter;
            }

        } else if(strcmp(line_contents[0], "cmp") == 0) {
            compare(machine, line_contents[1], line_contents[2]);
        } else if(strcmp(line_contents[0], "jmp") == 0) {
            machine->pc = line_contents[1];
        } else if(strcmp(line_contents[0], "jz") == 0) {
            if(machine->fl != 0) {
                machine->pc = line_contents[1];
            }
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
        } else if(strcmp(line_contents[0], "hlt") == 0) {
            halt(machine);
        } else if(strcmp(line_contents[0], "ret") == 0) {
            // TODO: return from function ?????
        } else if(strcmp(line_contents[0], "add") == 0) {
             
        } else if(strcmp(line_contents[0], "sub") == 0) {
            
        } else if(strcmp(line_contents[0], "mul") == 0) {
            
        } else if(strcmp(line_contents[0], "div") == 0) {
            
        } else {
            fprintf(stderr, " [COMPILATION] - unknown instruction!\n");
            ++err_counter;
        }

    }

    fclose(fp);
    fp = NULL;
    
    return err_counter;
}

int main(void) {

    machine_t* machine = (machine_t*) malloc(sizeof(machine_t));

    read_code(machine);

    free(machine);
    return 0;
}