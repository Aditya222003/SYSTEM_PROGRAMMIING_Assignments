#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include"debugger.h"
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

#define MAX_BREAKPOINTS 10
#define BUFFER_SIZE 256

struct breakpoint {
    long addr;
    long orig_data;
    int enabled;
};

struct breakpoint breakpoints[MAX_BREAKPOINTS];
int breakpoint_count = 0;

void set_breakpoint(pid_t pid, long addr) {
    for (int i = 0; i < MAX_BREAKPOINTS; i++) {
        if (!breakpoints[i].enabled) {
            breakpoints[i].addr = addr;
            breakpoints[i].orig_data = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
            ptrace(PTRACE_POKEDATA, pid, addr, (breakpoints[i].orig_data & ~0xFF) | 0xCC);
            breakpoints[i].enabled = 1;
            printf("Breakpoint set at address 0x%lx\n", addr);
            return;
        }
    }
    printf("No more available breakpoints\n");
}

void remove_breakpoint(pid_t pid, long addr) {
    for (int i = 0; i < MAX_BREAKPOINTS; i++) {
        if (breakpoints[i].enabled && breakpoints[i].addr == addr) {
            ptrace(PTRACE_POKEDATA, pid, addr, breakpoints[i].orig_data);
            breakpoints[i].enabled = 0;
            printf("Breakpoint removed at address 0x%lx\n", addr);
            return;
        }
    }
    printf("Breakpoint not found at address 0x%lx\n", addr);
}

void list_breakpoints() {
    printf("Breakpoints:\n");
    for (int i = 0; i < MAX_BREAKPOINTS; i++) {
        if (breakpoints[i].enabled) {
            printf("%d: Address 0x%lx\n", i+1, breakpoints[i].addr);
        }
    }
}

void print_registers(struct user_regs_struct regs) {
    printf("Registers:\n");
    printf("RIP: 0x%llx\n", regs.rip);
    printf("RAX: 0x%llx\n", regs.rax);
    printf("RBX: 0x%llx\n", regs.rbx);
    printf("RCX: 0x%llx\n", regs.rcx);
    printf("RDX: 0x%llx\n", regs.rdx);
    printf("RDI: 0x%llx\n", regs.rdi);
    printf("RSI: 0x%llx\n", regs.rsi);
    printf("RBP: 0x%llx\n", regs.rbp);
    printf("RSP: 0x%llx\n", regs.rsp);
    printf("R8 : 0x%llx\n", regs.r8);
    printf("R9 : 0x%llx\n", regs.r9);
    printf("R10: 0x%llx\n", regs.r10);
    printf("R11: 0x%llx\n", regs.r11);
    printf("R12: 0x%llx\n", regs.r12);
    printf("R13: 0x%llx\n", regs.r13);
    printf("R14: 0x%llx\n", regs.r14);
    printf("R15: 0x%llx\n", regs.r15);
}

void dump_memory(pid_t pid, unsigned long addr, int count) {
    unsigned long data;

    printf("Memory dump:\n");
    for (int i = 0; i < count; i++) {
        data = ptrace(PTRACE_PEEKDATA, pid, addr + i * sizeof(long), NULL);
        printf("0x%lx: 0x%lx\n", addr + i * sizeof(long), data);
    }
}

void handle_debugger(pid_t pid) {
    int status;
    struct user_regs_struct regs;
    char buffer[BUFFER_SIZE];

    printf("Debugger attached to process %d\n", pid);

    while (1) {
        printf("(dbg) ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if (strncmp(buffer, "continue", 8) == 0) {
            ptrace(PTRACE_CONT, pid, NULL, NULL);
            waitpid(pid, &status, 0);
        }
        else if (strncmp(buffer, "break", 5) == 0) {
            char *token = strtok(buffer, " ");
            token = strtok(NULL, " ");
            long addr = strtol(token, NULL, 16);
            set_breakpoint(pid, addr);
        }
        else if (strncmp(buffer, "remove", 6) == 0) {
            char *token = strtok(buffer, " ");
            token = strtok(NULL, " ");
            long addr = strtol(token, NULL, 16);
            remove_breakpoint(pid, addr);
        }
        else if (strncmp(buffer, "list", 4) == 0) {
            list_breakpoints();
        }
        else if (strncmp(buffer, "registers", 9) == 0) {
            ptrace(PTRACE_GETREGS, pid, NULL, &regs);
            print_registers(regs);
        }
        else if (strncmp(buffer, "memory", 6) == 0) {
            char *token = strtok(buffer, " ");
            token = strtok(NULL, " ");
            unsigned long addr = strtoul(token, NULL, 16);
            token = strtok(NULL, " ");
            int count = atoi(token);
            dump_memory(pid, addr, count);
        }
        else if (strncmp(buffer, "quit", 4) == 0) {
            printf("Detaching from process %d\n", pid);
            break;
        }
        else {
            printf("Unknown command\n");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <program>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Child process
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            perror("ptrace");
            exit(EXIT_FAILURE);
        }
        execl(argv[1], argv[1], NULL);
    } else {
        // Parent process
        handle_debugger(pid);
    }

    return 0;
}
