#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/pagedir.h"
#include "devices/shutdown.h"
#include <console.h>
#include "userprog/process.h"
#include "devices/input.h"

bool valid_addr(const void *ad)
{
  if(ad == NULL) return 0;
  if(is_kernel_vaddr(ad)) return 0;
  if(pagedir_get_page(thread_current()->pagedir, ad) == NULL) return 0;
  return 1;
}

void sys_halt(void) {
    shutdown_power_off();
}

void sys_exit(int status) {
    printf("%s: exit(%d)\n", thread_name(),  status);
    thread_current()->exit_status = status;
    thread_exit();
}

pid_t sys_exec(const char* cmd_line) {
    return process_execute(cmd_line);
}

int sys_wait(pid_t pid) {
    return process_wait(pid);
}

int sys_read(int fd, void* buffer, unsigned size) {
    int byte=0;
    if (fd == 0) {
        for (byte = 0; byte < (int)size; byte++)
            if (((char*)buffer)[byte] == '\0') break;
    }

    return byte;
}

int sys_write(int fd, const void* buffer, unsigned size) {
    if (fd == 1) {
        putbuf(buffer, size);
        return size;
    }

    return -1;
}

int sys_fibonacci(int n) {
    int f1 = 1;
    int f2 = 1;
    int temp;

    if (n == 1) return 1;
    if (n == 2) return 2;
    else {
        for (int i = 2; i < n; i++) {
            temp = f1 + f2;
            f1 = f2;
            f2 = temp;
        }
        return f2;
    }
}

int sys_max_of_four_integers(int a, int b, int c, int d) {
    int t1 = a>b? a: b;
    int t2 = c>d? c: d;
    return t1 > t2 ? t1 : t2;
}
static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
    int n, val1, val2, val3, val4;
    int* comd = f->esp;
    if (!valid_addr(f->esp)) sys_exit(-1);
    switch (*comd) {
        case SYS_HALT:
        {
            sys_halt();
            break;
        }
        case SYS_EXIT:
        {
            if (!valid_addr(comd+4))    sys_exit(-1);
            sys_exit(comd[1]);
            break;
        }
        case SYS_EXEC:
        {
            if (!valid_addr(comd+4))    sys_exit(-1);
                f->eax = sys_exec((const char*)comd[1]);
            break;
        }
        case SYS_WAIT:
        {
            if (!valid_addr(comd+4))    sys_exit(-1);
                f->eax = sys_wait(comd[1]);
            break;
        }
        case SYS_READ:
        {
            if (!valid_addr(comd+4)) sys_exit(-1);
            if (!valid_addr(comd+8)) sys_exit(-1);
            if (!valid_addr(comd+12)) sys_exit(-1);
            f->eax = sys_read(comd[1], (void*)comd[2], comd[3]);
            break;
        }
        case SYS_WRITE:
        {
            if (!valid_addr(comd+20)) sys_exit(-1);
            if (!valid_addr(comd+24)) sys_exit(-1);
            if (!valid_addr(comd+28)) sys_exit(-1);
            f->eax = sys_write(comd[5], (const void*)comd[6], comd[7]);
            break;
        }
        case SYS_FIBONACCI:
        {
            n = (int)comd[7];
            f->eax = sys_fibonacci(n);
            break;
        }
        case SYS_MAX_OF_FOUR_INTEGERS:
        {
            val1 = (int)comd[7];
            val2 = (int)comd[8];
            val3 = (int)comd[9];
            val4 = (int)comd[10];
            f->eax = sys_max_of_four_integers(val1, val2, val3, val4);
            break;
        }
    }
}
