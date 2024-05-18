#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100

typedef struct {
    char code[10];
    char mnemonic[10];
} Opcode;

void processInstruction(char *label, char *opcode, char *operand, int *locctr, FILE *fp3);
int findOpcode(char *opcode, Opcode *optab);
void writeSymbol(char *label, int locctr, FILE *fp3);

int main() {
    char line[MAX_LINE_LENGTH];
    char label[10], opcode[10], operand[10];
    int locctr = 0, start, length;
    
    FILE *fp1, *fp2, *fp3, *fp4;
    
    fp1 = fopen("input.txt", "r");
    fp2 = fopen("optab.txt", "r");
    fp3 = fopen("symtbl.txt", "w");
    fp4 = fopen("out.txt", "w");
    
    if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL) {
        printf("Error opening files.\n");
        return 1;
    }
    
    fgets(line, MAX_LINE_LENGTH, fp1);
    sscanf(line, "%s %s %s", label, opcode, operand);
    
    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);
        locctr = start;
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
        fgets(line, MAX_LINE_LENGTH, fp1); // Read next line
        sscanf(line, "%s %s %s", label, opcode, operand);
    } else {
        start = 0;
    }
    
    Opcode optab[100];
    int optabSize = 0;
    
    while (fscanf(fp2, "%s %s", optab[optabSize].code, optab[optabSize].mnemonic) != EOF) {
        optabSize++;
    }
    
    while (strcmp(opcode, "END") != 0) {
        processInstruction(label, opcode, operand, &locctr, fp3);
        fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        fgets(line, MAX_LINE_LENGTH, fp1); // Read next line
        sscanf(line, "%s %s %s", label, opcode, operand);
    }
    
    fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
    
    length = locctr - start;
    printf("The length of the code: %d\n", length);
    
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    
    return 0;
}

void processInstruction(char *label, char *opcode, char *operand, int *locctr, FILE *fp3) {
    if (strcmp(label, "**") != 0) {
        writeSymbol(label, *locctr, fp3);
    }
    
    if (strcmp(opcode, "WORD") == 0) {
        *locctr += 3;
    } else if (strcmp(opcode, "RESW") == 0) {
        *locctr += (3 * atoi(operand));
    } else if (strcmp(opcode, "RESB") == 0) {
        *locctr += atoi(operand);
    } else if (strcmp(opcode, "BYTE") == 0) {
        (*locctr)++;
    } else {
        Opcode optab[100];
        int optabSize = 0;
        int found = findOpcode(opcode, optab);
        
        if (found != -1) {
            (*locctr) += 3; 
        }
    }
}

int findOpcode(char *opcode, Opcode *optab) {
    int i;
    for (i = 0; i < 100; i++) {
        if (strcmp(opcode, optab[i].code) == 0) {
            return i;
        }
    }
    return -1; 
}

void writeSymbol(char *label, int locctr, FILE *fp3) {
    fprintf(fp3, "%s\t%d\n", label, locctr);
}
