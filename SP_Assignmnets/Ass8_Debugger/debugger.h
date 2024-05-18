#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <sys/types.h>
#include <sys/user.h>

void set_breakpoint(pid_t pid, long addr);
void remove_breakpoint(pid_t pid, long addr);
void list_breakpoints();
void print_registers(struct user_regs_struct regs);
void dump_memory(pid_t pid, unsigned long addr, int count);
void handle_debugger(pid_t pid);

#endif /* DEBUGGER_H */
