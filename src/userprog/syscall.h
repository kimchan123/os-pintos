#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
typedef int pid_t;
void syscall_init (void);
void syscall_exit (int status);
void sys_halt(void);
void sys_exit(int status);
pid_t sys_exec(const char* cmd_line);
int sys_wait(pid_t pid);
int sys_read(int fd, void* buffer, unsigned size);
int sys_write(int fd, const void* buffer, unsigned size);
int sys_fibonacci(int n);
int sys_max_of_four_integers(int a, int b, int c, int d);
#endif /* userprog/syscall.h */