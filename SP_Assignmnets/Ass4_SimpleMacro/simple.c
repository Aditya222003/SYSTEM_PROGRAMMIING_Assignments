#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int d_flag = 0;
int e_flag = 0;
char key_words[7][6] = {"load", "store", "add", "sub", "mul", "div", "END"};

void remove_char(char s, char **l) {
    int count = 0;
    for (int i = 0; l[i] != NULL; i++) {
        if (*l[i] == s) {
            count++;
        }
    }
    int j = 0;
    for (int i = 0; l[i] != NULL; i++) {
        if (*l[i] != s) {
            l[j++] = l[i];
        }
    }
    l[j] = NULL;
}

void remove_newline(char **l) {
    int length = 0;
    while (l[length + 1] != NULL) {
        length++;
    }
    char *s = l[length];
    s[strlen(s) - 1] = '\0';
}

void add_newline(char **l) {
    int length = 0;
    while (l[length + 1] != NULL) {
        length++;
    }
    char *s = l[length];
    strcat(s, "\n");
}

void replace_char(char **param, char **l) {
    int i = 0;
    while (l[i] != NULL) {
        for (int j = 0; j < 5; j++) {
            if (strcmp(l[i], param[j]) == 0) {
                char index[5];
                sprintf(index, "#%d", j + 1);
                strcpy(l[i], index);
            }
        }
        i++;
    }
}

void replace_char_2(char **l1, char **l2) {
    for (int i = 0; l2[i] != NULL; i++) {
        if (l2[i][0] == '#') {
            int c = atoi(&l2[i][1]);
            strcpy(l2[i], l1[c]);
        }
    }
}

void add_output(char **l, FILE *outpt) {
    add_newline(l);
    int i = 0;
    while (l[i] != NULL) {
        fprintf(outpt, "%s ", l[i]);
        i++;
    }
}

int main() {
    FILE *code, *out;
    code = fopen("c.txt", "r");
    out = fopen("out.txt", "w");
    char *mnt[100][3];
    char *mbt[100][10];
    char *parameter[5];

    while (1) {
        char line[100];
        if (fgets(line, sizeof(line), code) == NULL) {
            break;
        }
        char *token = strtok(line, " ");
        char *words[10];
        int i = 0;
        while (token != NULL) {
            words[i] = malloc(strlen(token) + 1);
            strcpy(words[i], token);
            token = strtok(NULL, " ");
            i++;
        }
        words[i - 1][strlen(words[i - 1]) - 1] = '\0'; // Remove newline character

        if (strcmp(words[0], "MACRO") == 0) {
            d_flag = 1;
            int parameter_count = i - 3;
            mnt[parameter_count][0] = malloc(strlen(words[1]) + 1);
            strcpy(mnt[parameter_count][0], words[1]);
            mnt[parameter_count][1] = malloc(3);
            sprintf(mnt[parameter_count][1], "%d", parameter_count);
            mnt[parameter_count][2] = malloc(3);
            sprintf(mnt[parameter_count][2], "%d", parameter_count);
            int count = 0;
            for (int j = 2; j < i; j++) {
                parameter[count] = malloc(strlen(words[j]) + 1);
                strcpy(parameter[count], words[j]);
                count++;
            }
        }
        if (d_flag == 1 && e_flag == 0 && strcmp(words[0], "MACRO") != 0) {
            replace_char(parameter, words);
            int index = atoi(mnt[i - 3][2]);
            for (int j = index; j < i - 1; j++) {
                for (int k = 0; k < 10; k++) {
                    mbt[j][k] = malloc(strlen(words[k]) + 1);
                    strcpy(mbt[j][k], words[k]);
                }
            }
        }
        if (d_flag == 1 && e_flag == 0 && strcmp(words[0], "MEND") == 0) {
            d_flag = 0;
            continue;
        }
        if (d_flag == 0 && e_flag == 0) {
            int is_keyword = 0;
            for (int j = 0; j < 7; j++) {
                if (strcmp(words[0], key_words[j]) == 0) {
                    is_keyword = 1;
                    break;
                }
            }
            if (is_keyword == 0) {
                e_flag = 1;
                for (int j = 0; j < 100; j++) {
                    if (strcmp(mnt[j][0], words[0]) == 0 && atoi(mnt[j][1]) == i - 1) {
                        int index = atoi(mnt[j][2]);
                        while (e_flag == 1 && d_flag == 0) {
                            for (int k = 0; mbt[index][k] != NULL; k++) {
                                replace_char_2(words, mbt[index]);
                                add_output(mbt[index], out);
                                index++;
                            }
                        }
                        break;
                    }
                }
                continue;
            }
            add_output(words, out);
        }
    }

    fclose(code);
    fclose(out);

    return 0;
}
