#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define NUM_REGISTERS 32
#define MEMORY_SIZE 4096

int registers[NUM_REGISTERS];
int memory[MEMORY_SIZE];
int pc = 200;

void parse_instruction(char *line, int *address, char *instruction, char args[3][10]) {
    sscanf(line, "%d %s %s %s %s", address, instruction, args[0], args[1], args[2]);
}


void execute_add(int dest_reg, int src_reg1, int src_reg2) {
    registers[dest_reg] = registers[src_reg1] + registers[src_reg2];
}

void execute_addi(int dest_reg, int src_reg, int immediate) {
    registers[dest_reg] = registers[src_reg] + immediate;
}

void execute_ldur(int dest_reg, int base_reg, int offset) {
    int address = registers[base_reg] + offset;
    registers[dest_reg] = memory[address];
}

void execute_stur(int src_reg, int base_reg, int offset) {
    int address = registers[base_reg] + offset;
    memory[address] = registers[src_reg];
}

int execute_b(int address) {
    return address;
}

void execute_logical(int dest_reg, int src_reg1, int src_reg2) {
    registers[dest_reg] = registers[src_reg1] & registers[src_reg2];
}

int execute_logical_branch(int src_reg1, int src_reg2, int address) {
    if (registers[src_reg1] == registers[src_reg2]) {
        return address;
    }
    return -1;
}

int main() {
    char file_name[100];
    printf("Enter the name of the file (e.g., 'code.txt'): ");
    scanf("%s", file_name);

    FILE *file = fopen(file_name, "r");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        int address;
        char instruction[10];
        char args[3][10] = {0};

        parse_instruction(line, &address, instruction, args);

        if (strcmp(instruction, "ADD") == 0) {
            execute_add(atoi(args[0] + 1), atoi(args[1] + 1), atoi(args[2] + 1));
        } else if (strcmp(instruction, "ADDI") == 0) {
            execute_addi(atoi(args[0] + 1), atoi(args[1] + 1), atoi(args[2] + 1));
        } else if (strcmp(instruction, "LDUR") == 0) {
            execute_ldur(atoi(args[0] + 1), atoi(args[1] + 1), atoi(args[2] + 1));
        } else if (strcmp(instruction, "STUR") == 0) {
            execute_stur(atoi(args[0] + 1), atoi(args[1] + 1), atoi(args[2] + 1));
        } else if (strcmp(instruction, "B") == 0) {
            pc = execute_b(address);
        } else if (strcmp(instruction, "AND") == 0) {
            execute_logical(atoi(args[0] + 1), atoi(args[1] + 1), atoi(args[2] + 1));
        } else if (strcmp(instruction, "BEQ") == 0) {
            int new_pc = execute_logical_branch(atoi(args[0] + 1), atoi(args[1] + 1), address);
            if (new_pc != -1) {
                pc = new_pc;
            }
        }

        printf("PC = %d, Instruction: %s", pc, line);
        printf("Registers: ");
        for (int i = 0; i < NUM_REGISTERS; i++) {
            printf("X%d = %d, ", i, registers[i]);
        }
        printf("\n");
    }

    printf("Final Memory State:\n");
    for (int i = 0; i < MEMORY_SIZE; i += 4) {
        printf("Memory[%d] = %d, ", i, memory[i]);
    }
    printf("\n");

    fclose(file);
    return 0;
}



