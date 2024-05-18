#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_OPCODE_LENGTH 10
#define MAX_OPERAND_LENGTH 10

// Symbol table entry structure
typedef struct {
    char label[MAX_OPERAND_LENGTH];
    int address;
} SymbolTableEntry;

// Literal table entry structure
typedef struct {
    char literal[MAX_OPERAND_LENGTH];
    int address;
} LiteralTableEntry;

// Function prototypes
void pass1(FILE *input_fp, FILE *symtab_fp, FILE *littab_fp);
void pass2(FILE *input_fp, FILE *symtab_fp, FILE *littab_fp, FILE *output_fp);

int main() {
    FILE *input_fp, *output_fp, *symtab_fp, *littab_fp;

    // Open input files for pass 1
    input_fp = fopen("input.txt", "r");
    symtab_fp = fopen("symtab.txt", "w");
    littab_fp = fopen("littab.txt", "w");

    if (input_fp == NULL || symtab_fp == NULL || littab_fp == NULL) {
        printf("Error: Unable to open input files.\n");
        return 1;
    }

    // Perform pass 1
    pass1(input_fp, symtab_fp, littab_fp);

    // Close input files after pass 1
    fclose(input_fp);
    fclose(symtab_fp);
    fclose(littab_fp);

    // Open input files for pass 2
    input_fp = fopen("input.txt", "r");
    symtab_fp = fopen("symtab.txt", "r");
    littab_fp = fopen("littab.txt", "r");
    output_fp = fopen("output.txt", "w");

    if (input_fp == NULL || symtab_fp == NULL || littab_fp == NULL || output_fp == NULL) {
        printf("Error: Unable to open input or output files.\n");
        return 1;
    }

    // Perform pass 2
    pass2(input_fp, symtab_fp, littab_fp, output_fp);

    // Close input and output files after pass 2
    fclose(input_fp);
    fclose(symtab_fp);
    fclose(littab_fp);
    fclose(output_fp);

    return 0;
}

// Pass 1 implementation
void pass1(FILE *input_fp, FILE *symtab_fp, FILE *littab_fp) {
    char line[MAX_LINE_LENGTH];
    char opcode[MAX_OPCODE_LENGTH];
    char operand[MAX_OPERAND_LENGTH];

    // Initialize symbol and literal table indices
    int symtab_index = 0;
    int littab_index = 0;

    while (fgets(line, sizeof(line), input_fp) != NULL) {
        sscanf(line, "%s %s", opcode, operand);

        // Process labels and literals
        if (operand[0] != '\0' && operand[0] != '\n') {
            if (operand[strlen(operand) - 1] == '\n')
                operand[strlen(operand) - 1] = '\0'; // Remove newline character

            if (operand[0] == '=') {
                // Add literal to literal table
                fprintf(littab_fp, "%s %d\n", operand, littab_index);
                littab_index++;
            } else {
                // Add label to symbol table
                fprintf(symtab_fp, "%s %d\n", operand, symtab_index);
                symtab_index++;
            }
        }
    }
}

// Pass 2 implementation
void pass2(FILE *input_fp, FILE *symtab_fp, FILE *littab_fp, FILE *output_fp) {
    char line[MAX_LINE_LENGTH];
    char opcode[MAX_OPCODE_LENGTH];
    char operand[MAX_OPERAND_LENGTH];

    while (fgets(line, sizeof(line), input_fp) != NULL) {
        sscanf(line, "%s %s", opcode, operand);

        // Generate machine code based on intermediate code
        // This part needs to be implemented based on the specific instructions and format
        // This is just a placeholder for demonstration purposes
        fprintf(output_fp, "Machine code for: %s %s\n", opcode, operand);
    }
}
