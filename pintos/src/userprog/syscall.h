#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
void syscall_exit(int status);
void syscall_close(int fd);

#endif /* userprog/syscall.h */
