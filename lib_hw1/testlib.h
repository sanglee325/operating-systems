#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_INPUT_LEN 100

// struct of list
struct list_item {
	struct list_elem elem;
	int data;
};

// table of list data_structure
struct list_table {
	char name[10];

	struct list* list;
	struct list_table* next;
};

struct list_table *list_tab_head;

void read_command(char *input_str);

void command_quit();

void create_bitmap(char *name);
void create_list(char *name);
void create_hashtable(char *name);
