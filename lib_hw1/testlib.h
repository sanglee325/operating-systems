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

// table of data_structure
struct ds_table {
	char name[10];

	//1: bitmap 2: hashtable 3: list
	int type;

	struct list* list;
	struct ds_table* next;
};

struct ds_table *ds_tab_head;

void read_command(char *input_str);

void command_quit();
void command_dumpdata(char *name);

void create_bitmap(char *name);
void create_list(char *name);
void create_hashtable(char *name);

void dump_list(struct list* list);

struct ds_table *find_ds_table (char *name);
