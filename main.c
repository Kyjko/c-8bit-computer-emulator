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

void read_code(machine_t* machine, const char* source_file_name) {

    // create relative path to source file
    char source_path[] = "./source/";
    strcat(source_path, source_file_name);

    reset(machine);
    #ifdef _DEBUG_
    printf("1 read_code()\n");
    #endif
    FILE* fp = fopen(source_path, "r");
    if(fp == NULL) {
        fprintf(stderr, "[-] fopen() - Cannot find the source file!\n");
        ++g_err_counter;
        return;
    }
    
    char buf[BUF_LEN];
    #ifdef _DEBUG_
    uint32_t ln = 0;
    #endif
    while(fgets(buf, BUF_LEN, fp)) {
        // only look at non-empty lines
        //remove trailing newline
        buf[strcspn(buf, "\n")] = 0;
        if(strcmp(buf, "\n") != 0) {

        #ifdef _DEBUG_
        printf("BUF: %s\n", buf);
        #endif
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
            #ifdef _DEBUG_
            fprintf(stdout, "LINE #%d: %s\n", ++ln, buf);
            #endif
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

    // default source file name
    char source_file_name[] = "source.kyasm";
    // set verbosity to 0 by default
    set_verbosity(machine, 0);
    
    if(argc > 1) {
        
        if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
            // help
            fprintf(stdout, "Usage: main.exe [-S <filename>] [-V]\n\t-V: verbose output\n\t-S <filename>: specify assembly source file\n");
            return 0;
        }

        uint32_t i = 1;
        while(i < argc) {
            // search through optional arguments in argv
            if(strcmp(argv[i], "-V") == 0) {
                set_verbosity(machine, 1);
            }
            if(strcmp(argv[i], "-S") == 0) {
            // use custom source file for assembly code
                if(argv[i+1] != NULL && strlen(argv[i+1]) == 0) {
                    fprintf(stderr, "[-] - source file name cannot be empty!\n");
                    return -1;
                } else {
                    strcpy(source_file_name, argv[2]);
                }
            }

            ++i;
        }
    }
    
    #ifdef _DEBUG_
    printf("1 main()\n");
    #endif

    read_code(machine, source_file_name);
    
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

    fprintf(stdout, "enter any key to continue...\n");
    fgetc(stdin);
    return 0;
}