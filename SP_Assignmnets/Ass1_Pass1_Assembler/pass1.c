#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_OPCODE_LEN 10
#define MAX_OPERAND_LEN 10
#define MAX_LABEL_LEN 10

typedef struct {
    char opcode[MAX_OPCODE_LEN];
    char operand[MAX_OPERAND_LEN];
    int address;
} Instruction;

typedef struct {
    char name[MAX_LABEL_LEN];
    int address;
} Label;

const char *supported_instructions[] = {"MOV", "ADD", "SUB", "JMP", "HLT", "LDA", "STA", "LDCH", "STCH"};
void process_instructions(FILE *input_fp, FILE *output_fp, FILE *symbol_fp);
int get_instruction_address(Label *labels, int num_labels, char *label_name);

int main() {
    FILE *input_fp, *output_fp, *symbol_fp;

    input_fp = fopen("input.txt", "r");
    output_fp = fopen("out.txt", "w");
    symbol_fp = fopen("symtbl.txt", "w");

    if (input_fp == NULL || output_fp == NULL || symbol_fp == NULL) {
        printf("Error: Unable to open files.\n");
        return 1;
    }

    process_instructions(input_fp, output_fp, symbol_fp);

    fclose(input_fp);
    fclose(output_fp);
    fclose(symbol_fp);

    return 0;
}

void process_instructions(FILE *input_fp, FILE *output_fp, FILE *symbol_fp) {
    Instruction instr;
    Label labels[100]; 
    int num_labels = 0;
    int locctr = 0;
    char label[MAX_LABEL_LEN];

    fscanf(input_fp, "%s %s %s", label, instr.opcode, instr.operand);
    if (strcmp(instr.opcode, "START") == 0) {
        locctr = atoi(instr.operand);
        fprintf(output_fp, "%d\t%s\t%s\n", locctr, instr.opcode, instr.operand);
        fscanf(input_fp, "%s %s %s", label, instr.opcode, instr.operand);
    }

    while (strcmp(instr.opcode, "END") != 0) {
        fprintf(output_fp, "%d\t%s\t%s\n", locctr, instr.opcode, instr.operand);

        if (strcmp(label, "**") != 0) {
            strcpy(labels[num_labels].name, label);
            labels[num_labels].address = locctr;
            fprintf(symbol_fp, "%s\t%d\n", label, locctr);
            num_labels++;
        }

        if (strcmp(instr.opcode, "WORD") == 0) {
            locctr += 3;
        } else if (strcmp(instr.opcode, "RESW") == 0) {
            locctr += 3 * atoi(instr.operand);
        } else if (strcmp(instr.opcode, "RESB") == 0) {
            locctr += atoi(instr.operand);
        } else if (strcmp(instr.opcode, "BYTE") == 0) {
            locctr++;
        } else {
            int i;
            for (i = 0; i < sizeof(supported_instructions) / sizeof(supported_instructions[0]); i++) {
                if (strcmp(instr.opcode, supported_instructions[i]) == 0) {
                    locctr += 3; 
                    break;
                }
            }
            if (i == sizeof(supported_instructions) / sizeof(supported_instructions[0])) {
                printf("Error: Unsupported instruction '%s'\n", instr.opcode);
                return;
            }
        }

        fscanf(input_fp, "%s %s %s", label, instr.opcode, instr.operand);
    }

    fprintf(output_fp, "%d\t%s\t%s\n", locctr, instr.opcode, instr.operand);
}

int get_instruction_address(Label *labels, int num_labels, char *label_name) {
    int i;
    for (i = 0; i < num_labels; i++) {
        if (strcmp(labels[i].name, label_name) == 0) {
            return labels[i].address;
        }
    }
    return -1; 
}
