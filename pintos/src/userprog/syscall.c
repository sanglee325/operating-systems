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
#include <string.h>
#include "threads/synch.h"

static void syscall_handler (struct intr_frame *);

/* implemeted functions */
/* project 1 */
void syscall_halt(void);
//void syscall_exit(int status);
pid_t syscall_exec(const char* cmd_line);
int syscall_wait(pid_t pid);
int syscall_read(int fd, void *buffer, unsigned size);
int syscall_write(int fd, const void *buffer, unsigned size);

/* project 2 */
bool syscall_create(const char *file, unsigned initial_size);
bool syscall_remove(const char *file);
int syscall_open(const char *file);
int syscall_filesize(int fd);
void syscall_seek(int fd, unsigned position);
unsigned syscall_tell(int fd);
void syscall_close(int fd);

/* additional system calls */
int syscall_fibonacci(int n);
int syscall_sum_of_four_int(int a, int b, int c, int d);

bool check_valid(const void* uaddr);

struct lock filesys_lock;

void
syscall_init (void) 
{
  lock_init(&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f) 
{
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
		else syscall_exit(*(uint32_t *)(f->esp + 4)); 
		break;
	case SYS_EXEC:                   /* Start another process. */
		if (!check_valid(f->esp + 4) || 
				!check_valid((const char*)*(uint32_t*)(f->esp + 4))) syscall_exit(-1);
		else f->eax = syscall_exec((const char*)*(uint32_t*)(f->esp + 4));
		break;
	case SYS_WAIT:                   /* Wait for a child process to die. */
		if (!check_valid(f->esp + 4)) syscall_exit(-1);
		else f->eax = syscall_wait((pid_t)*(uint32_t *)(f->esp + 4));
		break;
	case SYS_CREATE:                 /* Create a file. */
		if (!check_valid(f->esp + 4) || !check_valid(f->esp + 8)) 
			syscall_exit(-1);
		else f->eax = syscall_create((const char*)*(uint32_t *)(f->esp + 4), 
				(unsigned)*(uint32_t *)(f->esp + 8));
		break;
	case SYS_REMOVE:                 /* Delete a file. */
		if (!check_valid(f->esp + 4)) 
			syscall_exit(-1);
		else f->eax = syscall_remove((const char*)*(uint32_t *)(f->esp + 4));
		break;
	case SYS_OPEN:	                 /* Open a file. */
		if (!check_valid(f->esp + 4)) 
			syscall_exit(-1);
		else f->eax = syscall_open((const char*)*(uint32_t *)(f->esp + 4));
		break;
	case SYS_FILESIZE:               /* Obtain a file's size. */
		if (!check_valid(f->esp + 4)) 
			syscall_exit(-1);
		else f->eax = syscall_filesize((int)*(uint32_t *)(f->esp + 4));
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
		if (!check_valid(f->esp + 4) || !check_valid(f->esp + 8)) 
			syscall_exit(-1);
		else syscall_seek((int)*(uint32_t *)(f->esp + 4), 
				(unsigned)*(uint32_t *)(f->esp + 8));
		break;
	case SYS_TELL:                   /* Report current position in a file. */
		if (!check_valid(f->esp + 4)) 
			syscall_exit(-1);
		else f->eax = syscall_tell((int)*(uint32_t *)(f->esp + 4));
		break;
	case SYS_CLOSE:                  /* Close a file. */
		if (!check_valid(f->esp + 4)) 
			syscall_exit(-1);
		else syscall_close((int)*(uint32_t *)(f->esp + 4));
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

	/* additional system call */
	case SYS_FIBO:					/* Returns Nth value of fibonacci sequence. */
		if (!check_valid(f->esp + 4)) syscall_exit(-1);
		else f->eax = syscall_fibonacci((int)*(uint32_t *)(f->esp + 4)); 
		break;
	case SYS_SUM_FOUR:				/* Returns the sum of four. */
		if (!check_valid(f->esp + 4) || !check_valid(f->esp + 8) 
				|| !check_valid(f->esp + 12) || !check_valid(f->esp + 16)) 
			syscall_exit(-1);
		else f->eax = syscall_sum_of_four_int((int)*(uint32_t *)(f->esp + 4), 
				(int)*(uint32_t *)(f->esp + 8), (int)*(uint32_t *)(f->esp + 12), 
				(int)*(uint32_t *)(f->esp + 16));
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
bool check_fd_value(const int fd) {
	if (0 <= fd && fd < 128) return true;
	return false;
}
void syscall_halt(void) {
	shutdown_power_off();
}
void syscall_exit(int status) {
	int i;
	struct thread *cur = thread_current();
	printf("%s: exit(%d)\n", cur->name, status);
	thread_current()->exit_status = status;

	for (i = 2; i < 128; i++) {
		if (cur->fd[i] != NULL) {
			syscall_close(i);
		}
	}

	thread_exit();
}
pid_t syscall_exec(const char* cmd_line) {
	if (!thread_current()->load_status)
		return TID_ERROR;
	return process_execute(cmd_line);
}
int syscall_wait(pid_t pid) {
	return process_wait(pid);
}
int syscall_read(int fd, void *buffer, unsigned size) {
	// check if memory is valid
	int i;
	int ret = -1;
	struct file *fp;

	if(!check_fd_value(fd)) return ret;
	if(!check_valid(buffer) || !check_valid(buffer+size))
	//if(is_kernel_vaddr(buffer))
	//if (buffer >= PHYS_BASE)
		syscall_exit(-1);

	lock_acquire(&filesys_lock);
	if(fd == 0) {
		for(i = 0; i < (int)size; i++) {
			*((char*) buffer + i) = input_getc();
		}
		ret = i;
	}

	else if(fd >= 2) {
		fp = thread_current()->fd[fd];
		if (fp == NULL) {
			lock_release(&filesys_lock);
			return ret;
		}
		else {
			ret = file_read(fp, buffer, size);
		}
	}
	lock_release(&filesys_lock);

	return ret;

}
int syscall_write(int fd, const void *buffer, unsigned size) {
	// check if memory is valid
	//int i;
	int ret = 0;
	struct file *fp;

	if(!check_fd_value(fd)) return ret;
	if(!check_valid(buffer) || !check_valid(buffer+size))
	//if(is_kernel_vaddr(buffer))
		syscall_exit(-1);

	lock_acquire(&filesys_lock);
	if(fd == 1) {
		putbuf(buffer, size);
		ret = size;
	}

	else if(fd >= 2) {
		fp = thread_current()->fd[fd];
		if (fp == NULL) {
			lock_release(&filesys_lock);
			return ret;
		}
		else {
			ret = file_write(fp, buffer, size);
		}
	}
	lock_release(&filesys_lock);

	return ret;
}

/* additional system calls */
int syscall_fibonacci(int n) {
	int i;
	int x = 0, y = 1;
	int fibo;

	if(n == 0)
		fibo = x;
	else if (n == 1)
		fibo = y;
	else {
		fibo = x;
		for(i = 1; i < n; i++) {
			fibo = x + y;
			x = y;
			y = fibo;
		}
	}

	return fibo;
}
int syscall_sum_of_four_int(int a, int b, int c, int d) {
	return a + b + c + d;
}
bool syscall_create(const char *file, unsigned initial_size) {
	if(!check_valid(file)) 
		syscall_exit(-1);

	return filesys_create(file, initial_size);
}
bool syscall_remove(const char *file) {
	if(!check_valid(file)) 
		syscall_exit(-1);

	return filesys_remove(file);
}
int syscall_open(const char *file) {
	struct file* f;
	int idx = 2;

	if(!check_valid(file)) 
		syscall_exit(-1);

	lock_acquire(&filesys_lock);
	f = filesys_open(file);
	if(f == NULL) {
		lock_release(&filesys_lock);
		return -1;
	}
	else {
		while(thread_current()->fd[idx]) {
			idx++;
			if(idx > 127 || idx < 0) {
				lock_release(&filesys_lock);
				return -1;
			}
		}
		thread_current()->fd[idx] = f;

		if(!strcmp(thread_current()->name, file)) {
			file_deny_write(f);
		}
		else {
			file_allow_write(f);
		}
	}
	lock_release(&filesys_lock);

	return idx;
}
int syscall_filesize(int fd) {
	if(!check_fd_value(fd)) return 0;  // value...?
	return file_length(thread_current()->fd[fd]);
}
void syscall_seek(int fd, unsigned position) {
	if(!check_fd_value(fd)) return ;
	file_seek(thread_current()->fd[fd], position);
}
unsigned syscall_tell(int fd) {
	if(!check_fd_value(fd)) return 0;
	return file_tell(thread_current()->fd[fd]);
}
void syscall_close(int fd) {
	struct file *fp;
	
	if(!check_fd_value(fd)) return;
	if(thread_current()->fd[fd] == NULL)
		//return;
		syscall_exit(-1);
	fp = thread_current()->fd[fd];
	thread_current()->fd[fd] = NULL;
	file_close(fp);
}

