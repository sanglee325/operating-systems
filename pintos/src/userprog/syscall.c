#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#include "lib/user/syscall.h"
#include "threads/vaddr.h"
#include "threads/palloc.h"
#include "devices/shutdown.h"
#include "devices/input.h"
#include "userprog/process.h"
#include "userprog/pagedir.h"

#include "filesys/filesys.h"
#include "filesys/file.h"

static void syscall_handler (struct intr_frame *);

/* implemeted functions */
void syscall_halt(void);
void syscall_exit(int status);
pid_t syscall_exec(const char* cmd_line);
int syscall_wait(pid_t pid);
int syscall_read(int fd, void *buffer, unsigned size);
int syscall_write(int fd, const void *buffer, unsigned size);

/* additional system calls */
int fibonacci(int n);
int sum_of_four_int(int a, int b, int c, int d);

bool check_valid(const void* uaddr);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
  int arg[20];
//  printf ("system call!\n");
//  printf("syscall: %d\n", *(uint32_t*)(f->esp));
//  hex_dump(f->esp, f->esp, 200, true);
   
  if (!check_valid(f->esp)) syscall_exit(-1);
  switch(*(uint32_t *)(f->esp)) {
    /* Projects 2 and later. */
	case SYS_HALT:                   /* Halt the operating system. */
		syscall_halt();
		break;
	case SYS_EXIT: 	                 /* Terminate this process. */
		if (!check_valid(f->esp + 4)) syscall_exit(-1);
		else syscall_exit(*(uint32_t *)(f->esp + 4)); //16
		break;
	case SYS_EXEC:                   /* Start another process. */
		if (!check_valid(f->esp + 4) || 
				!check_valid((const char*)*(uint32_t*)(f->esp + 4))) syscall_exit(-1);
		else f->eax = syscall_exec((const char*)*(uint32_t*)(f->esp + 4));
		//else f->eax = syscall_exec(*(const char**)(f->esp + 4));
		break;
	case SYS_WAIT:                   /* Wait for a child process to die. */
		if (!check_valid(f->esp + 4)) syscall_exit(-1);
		else f->eax = syscall_wait((pid_t)*(uint32_t *)(f->esp + 4));
		break;
	case SYS_CREATE:                 /* Create a file. */
		break;
	case SYS_REMOVE:                 /* Delete a file. */
		break;
	case SYS_OPEN:	                 /* Open a file. */
		break;
	case SYS_FILESIZE:               /* Obtain a file's size. */
		break;
	case SYS_READ:                   /* Read from a file. */
		if (!check_valid(f->esp + 4) || !check_valid(f->esp + 8) || !check_valid(f->esp + 12)) 
			syscall_exit(-1);
		else f->eax = syscall_read((int)*(uint32_t *)(f->esp + 4), 
				(void*)*(uint32_t *)(f->esp + 8), (unsigned)*(uint32_t *)(f->esp + 12));
		break;
	case SYS_WRITE:                  /* Write to a file. */
		if (!check_valid(f->esp + 4) || !check_valid(f->esp + 8) || !check_valid(f->esp + 12)) 
			syscall_exit(-1);
		else f->eax = syscall_write((int)*(uint32_t *)(f->esp + 4), 
				(void*)*(uint32_t *)(f->esp + 8), (unsigned)*(uint32_t *)(f->esp + 12)); //16
		break;
	case SYS_SEEK:                   /* Change position in a file. */
		break;
	case SYS_TELL:                   /* Report current position in a file. */
		break;
	case SYS_CLOSE:                  /* Close a file. */
		break;

    /* Project 3 and optionally project 4. */
	case SYS_MMAP:                   /* Map a file into memory. */
		break;
	case SYS_MUNMAP:                 /* Remove a memory mapping. */
		break;

    /* Project 4 only. */
	case SYS_CHDIR:                  /* Change the current directory. */
		break;
	case SYS_MKDIR:                  /* Create a directory. */
		break;
	case SYS_READDIR:                /* Reads a directory entry. */
		break;
	case SYS_ISDIR:                  /* Tests if a fd represents a directory. */
		break;
	case SYS_INUMBER:                /* Returns the inode number for a fd. */
		break;
	default:
		thread_exit();
  }
}
bool check_valid(const void* uaddr) {
	if(uaddr == NULL) return false;
	if(!is_user_vaddr(uaddr)) return false;
	if(pagedir_get_page(thread_current()->pagedir, uaddr) == NULL) return false;

	return true;
}
void syscall_halt(void) {
	shutdown_power_off();
}
void syscall_exit(int status) {
	struct thread *cur = thread_current();
	printf("%s: exit(%d)\n", cur->name, status);
	thread_current()->exit_status = status;
	thread_exit();
}
pid_t syscall_exec(const char* cmd_line) {
	return process_execute(cmd_line);
}
int syscall_wait(pid_t pid) {
	return process_wait(pid);
}
int syscall_read(int fd, void *buffer, unsigned size) {
	// check if memory is valid
	int i;
	int ret = -1;

	if(fd == 0) {
		for(i = 0; i < size; i++) {
			*((char*) buffer + i) = input_getc();
		}
		ret = i;
	}

	return ret;

}
int syscall_write(int fd, const void *buffer, unsigned size) {
	// check if memory is valid
	int i;
	int ret = 0;

	if(fd == 1) {
		/*
		for(i = 0; i < size; i++) {
			printf("%c",*((char*) buffer + 4 + i));
		}*/
		putbuf(buffer, size);
		ret = size;
	}

	return ret;
}

/* additional system calls */
int fibonacci(int n);
int sum_of_four_int(int a, int b, int c, int d);

