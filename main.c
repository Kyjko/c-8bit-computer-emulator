/*
    main.c - Bognár Miklós 2021
    Parse custom assembly code and drive emulated machine

    TODO: function syntax and jmp/jz in code lines

*/

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "machine.c"
#include <stdlib.h>
#include <stdbool.h>

// assume one line doesn't contain more than 8 bits worth of chars
#define BUF_LEN (2<<9)-1
#define MAX_LINE_ELEMENTS 50
/*
    Supported instructions: (...) -> to be implemented
    mov, cmp, jmp, jz, pop, push, (lea), nop, hlt, (ret), add, sub, mul, div
    
*/

uint32_t g_err_counter = 0;

void read_code(machine_t* machine) {
    reset(machine);
    #ifdef _DEBUG_
    printf("1 read_code()\n");
    #endif
    FILE* fp = fopen("./source/source.kyasm", "r");
    if(fp == NULL) {
        fprintf(stderr, "[-] fopen() - Cannot find the source file!\n");
        return;
    }
    
    char buf[BUF_LEN];
    uint32_t ln = 0;
    while(fgets(buf, BUF_LEN, fp)) {
        // only look at non-empty lines
        //remove trailing newline
        buf[strcspn(buf, "\n")] = 0;
        if(strcmp(buf, "\n") != 0) {

        printf("BUF: %s\n", buf);
        char* line_contents[MAX_LINE_ELEMENTS];
        uint8_t line_elem_counter = 0;

        char buf2[BUF_LEN];
        strcpy(buf2, buf);

        char* tok = strtok(buf2, delim);
        while(tok != NULL) {
            line_contents[line_elem_counter++] = tok;
            tok = strtok(NULL, delim);
        }

        if(strlen(buf) != 0 && strcmp(line_contents[0], ";") != 0) {
            fprintf(stdout, "LINE #%d: %s\n", ++ln, buf);
            add_to_program_memory(machine, buf);
        }
        }
    }

    fclose(fp);
    fp = NULL;
    
    #ifdef _DEBUG_
    printf("2 read_code()\n");
    #endif

    g_err_counter = execute_program(machine);

}

int main(int argc, char** argv) {
    
    machine_t* machine = (machine_t*) malloc(sizeof(machine_t));
    
    #ifdef _DEBUG_
    printf("1 main()\n");
    #endif
    read_code(machine);
    
    if(g_err_counter != 0) {
        fprintf(stderr, "[===> CODE EXECUTION <===] - ERROR(S)!\n");
        fprintf(stderr, "Errors: %d\n", g_err_counter);
        free(machine);
        return -1;
    } else {
        fprintf(stderr, "[===> CODE EXECUTION <===] - SUCCESS!\n");
        show_screen_output(machine);
        // TODO: implement this function!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    }

    #ifdef _DEBUG_
    printf("2 main()\n");
    #endif

    free(machine);
    return 0;
}