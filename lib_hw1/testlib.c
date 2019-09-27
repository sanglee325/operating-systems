#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include "testlib.h"

char *input_str;
bool exit_flag = false;

int main() {

	while(1){
		input_str = (char*)calloc((MAX_INPUT_LEN), sizeof(char));
		
		fgets(input_str, MAX_INPUT_LEN, stdin);
		read_command(input_str);
		free(input_str);

		if(exit_flag ==  true) break;
	}

	return 0;
}

void read_command(char *input_str) {
	// variables for reading commands
	int i = 0;
	int idx = 0, word_num = 0, token_idx = 0;
	char *command;
	char tokenize[MAX_INPUT_LEN][MAX_INPUT_LEN] = { 0, };
	bool word[MAX_INPUT_LEN] = { false };
	
	for(i = 0; i < strlen(input_str); i++) {
		if(input_str[i] != ' ' && input_str[i] != '\n') word[i] = true;
	}

	for(i = 1; i < strlen(input_str); i++) {
		if(word[i-1] == true && word[i] == false) {
			word_num++;
		}
	}

	if(word_num > 0) {
		idx = 0;
		for(i = 0; i < strlen(input_str); i++) {
			if(input_str[i] && word[i] == true) {
				tokenize[idx][token_idx] = input_str[i];
				token_idx++;
			}
			if(word[i] == true && word[i+1] == false) {
				idx++;
				token_idx = 0;
			}
			if(idx == word_num) break;
		}
	}

	command = tokenize[0];
	
	// input token check
	/*
	printf("%s\n", command);
	for(i = 0; i<idx; i++) {
		printf("%s\n", tokenize[i]);
	}
	*/	

	if(!strcmp(command, "quit")) {
		command_quit();
	}
	else if(!strcmp(command, "create")){
		if(!strcmp(tokenize[1], "bitmap")) {
			create_bitmap(tokenize[2]);
		}
		else if(!strcmp(tokenize[1], "list")) {
			create_list(tokenize[2]);
		}
		else if(!strcmp(tokenize[1], "hashtable")) {
			create_list(tokenize[2]);
		}
	}
	else if(!strcmp(command, "dumpdata")) {
		command_dumpdata(tokenize[1]);
	}
	else if(!strcmp(command, "list_insert")) {
		struct ds_table *target = NULL;
		int idx = atoi(tokenize[2]);
		int data = atoi(tokenize[3]);

		// find the name data structure
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		// new data to insert
		struct list_item *new = malloc(sizeof(struct list_item));

		new->data = data;

		struct list *cur = target->list;
		struct list_elem *ptr = list_head(cur);

		for(int i = 0; i <= idx; i++) {
			ptr = list_next(ptr);
		}
		
		list_insert(ptr, &(new->elem));
		
	}

}

void command_quit() {
	if(ds_tab_head) {
	}

	exit_flag = true;
}

void create_bitmap(char *name) {
	
}

void create_list(char *name) {
	struct ds_table *new, *cur;

	new = (struct ds_table*)malloc(sizeof(struct ds_table));
	list_init(new->list);
	strcpy(new->name, name);
	new->type = 3;
	new->next = NULL;

	if(!ds_tab_head) {
		ds_tab_head = new;
	}
	else {
		cur = ds_tab_head;
		while(1) {
			if(cur->next) {
				cur = cur->next;
			}
			else {
				cur->next = new;
				break;
			}
		}
	}
}

void create_hashtable(char *name) {
	
}

struct ds_table *find_ds_table (char *name) {
	struct ds_table *cur;
	cur = ds_tab_head;

	printf("FUNC: find_ds_table(%s)\n", name);

	while(1) {
		if(!strcmp(cur->name, name)) {
			return cur;
		}
		else {
			cur = cur->next;
			if(cur == NULL) {
				printf("ERROR: %s does not exists.\n", name);
				break;
			}
		}
	}

	return NULL;
}

void command_dumpdata(char *name) {
	struct ds_table *target = NULL;


	target = find_ds_table(name);
	if(target == NULL) return;

	switch(target->type) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			dump_list(target->list);
			break;
	}
}

void dump_list(struct list* list) {
	if(list_empty(list))
		return;
	else {
		struct list_elem *cur;
		struct list_item *target;

		for(cur = list_begin(list); cur != list_end(list); cur = list_next(cur)) {
			target = list_entry(cur, struct list_item, elem);
			printf("%d ", target->data);
		}
		printf("\n");
	}
}
