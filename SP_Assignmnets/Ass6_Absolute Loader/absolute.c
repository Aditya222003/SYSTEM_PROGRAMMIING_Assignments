#include <stdio.h>
#include <string.h>

FILE *f1, *f2;
char input[20], label[20];
int length = 0, l, addr = 0, taddr, start, address, end, ptaddr = 0, w = 0, count = 0, i = 0;

void check() {
    count++;
    address++;
    taddr++;
    if (count == 4) {
        fprintf(f2, " ");
        i++;
        if (i == 4) {
            fprintf(f2, "\n%x\t\t\t", taddr);
            i = 0;
        }
        count = 0;
    }
}

int main() {
    int printed_addresses = 0;
    f1 = fopen("abs.txt", "r");
    f2 = fopen("output.txt", "w+");

    fscanf(f1, "%s", input);
    fprintf(f2, "MEMORY ADDRESS\t\t\t\tCONTENTS\n");

    while (strncmp(input, "E", 1) != 0 && printed_addresses < 15) {
        if (strncmp(input, "H", 1) == 0) {
            fscanf(f1, "%s%x%x%s", label, &start, &end, input);
            address = start;
        } else if (strncmp(input, "T", 1) == 0) {
            l = length;
            ptaddr = addr;
            fscanf(f1, "%x%x%s", &taddr, &length, input);
            addr = taddr;

            if (w == 0) {
                ptaddr = address;
                w = 1;
            }

            for (int k = 0; k < (taddr - (ptaddr + l)); k++) {
                address++;
                fprintf(f2, "xx");
                count++;
                if (count == 4) {
                    fprintf(f2, " ");
                    i++;
                    if (i == 4) {
                        fprintf(f2, "\n%x\t\t\t", address);
                        i = 0;
                    }
                    count = 0;
                }
            }

            if (taddr == start) {
                fprintf(f2, "\n%x\t\t\t", taddr);
            }

            fprintf(f2, "%c%c%c%c%c%c", input[0], input[1], input[2], input[3], input[4], input[5]);
            check();
            fscanf(f1, "%s", input);
        }
        // Check if a new address has been printed
        if (strncmp(input, "T", 1) == 0 && taddr != start) {
            printed_addresses++;
        } 
        else {
            fprintf(f2, "%c%c%c%c%c%c", input[0], input[1], input[2], input[3], input[4], input[5]);
            check();
            fscanf(f1, "%s", input);
        }
    }

    while (i != 4) {
        fprintf(f2, "xx");
        count++;
        if (count == 4) {
            fprintf(f2, " ");
            i++;
            count = 0;
        }
    }

    fclose(f1);
    fclose(f2);

    return 0;
}
