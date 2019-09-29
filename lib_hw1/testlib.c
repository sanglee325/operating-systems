#include "bitmap.h"
#include "hash.h"
#include "list.h"

#include "testlib.h"
#include "test_list.h"
#include "test_hash.h"

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
	
	// tokenizing of input command
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
	
	// commands start
	if(!strcmp(command, "quit")) {
		command_quit();
	}
	else if(!strcmp(command, "create")) {
		if(!strcmp(tokenize[1], "bitmap")) {
			create_bitmap(tokenize[2]);
		}
		else if(!strcmp(tokenize[1], "list")) {
			create_list(tokenize[2]);
		}
		else if(!strcmp(tokenize[1], "hashtable")) {
			create_hashtable(tokenize[2]);
		}
	}

	else if(!strcmp(command, "delete")){
		command_delete(tokenize[1]);
	}

	else if(!strcmp(command, "dumpdata")) {
		command_dumpdata(tokenize[1]);
	}

	else if(!strncmp(command, "list_", 5)) {
		command_list(command, tokenize, word_num);
	}

	else if(!strncmp(command, "hash_", 5)) {
		command_hash(command, tokenize, word_num);
	}
}

void command_quit() {
	exit_flag = true;
}

void create_bitmap(char *name) {
}

void create_list(char *name) {
	struct ds_table *new, *cur = NULL;

	new = (struct ds_table*)malloc(sizeof(struct ds_table));
	new->list = (struct list*)malloc(sizeof(struct list));
	list_init(new->list);
	strcpy(new->name, name);
	new->type = 3;
	new->next = NULL;
	new->prev = NULL;

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
				new->prev = cur;
				break;
			}
		}
	}
}

void create_hashtable(char *name) {
	struct ds_table *new, *cur = NULL;

	new = (struct ds_table*)malloc(sizeof(struct ds_table));
	new->hash = (struct hash*)malloc(sizeof(struct hash));
	hash_init(new->hash, hash_value_elem, less_value_hash, NULL);
	strcpy(new->name, name);
	new->type = 2;
	new->next = NULL;
	new->prev = NULL;

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
				new->prev = cur;
				break;
			}
		}
	}
	
	
}

struct ds_table *find_ds_table (char *name) {
	struct ds_table *cur;
	cur = ds_tab_head;

	if(cur == NULL) return NULL;
	
	while(1) {
		if(!strcmp(cur->name, name)) {
			return cur;
		}
		else {
			cur = cur->next;
			if(cur == NULL) {
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
			dump_hash(target->hash);
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

void dump_hash(struct hash* hash) {
	if(hash_empty(hash))
		return;
	else {
		struct hash_iterator *iter;
		hash_first(iter, hash);

		while(hash_next(iter)) {
			struct hash_item *item;
			item = hash_entry(hash_cur(iter), struct hash_item, elem);
			
			printf("%d ", item->data);
		}
		printf("\n");
	}
}

void command_delete(char *name) {
	struct ds_table *target = NULL;

	target = find_ds_table(name);
	if(target == NULL) return;

	switch(target->type) {
		case 1:
			break;
		case 2:
			delete_hash(target);
			break;
		case 3:
			delete_list(target);
			break;
	}
}

void delete_list(struct ds_table* target) {
	struct list_item *tar_item;

	// free elements of list
	while(!list_empty(target->list)) {
		struct list_elem *e = list_pop_front(target->list);
		
		tar_item = list_entry(e, struct list_item, elem);
		free(tar_item);
	}

	//free list
	free(target->list);
	delete_ds_table(target);
}

void delete_hash(struct ds_table* target) {
	struct hash_item *tar_item;

	// free elements of hash
	hash_destroy(target->hash, h_hash_free); 

	//free hash
	free(target->hash);
	delete_ds_table(target);
}

void delete_ds_table(struct ds_table* target) {
	struct ds_table *tar_prev, *tar_next;

	// check for data struct table status and free it
	// case: only one ds left
	if(target->prev == NULL && target->next == NULL) {
		ds_tab_head = NULL;
		free(target);
	}

	// first element of table
	else if(target->prev == NULL && target->next != NULL) {
		tar_next = target->next;

		ds_tab_head = target->next;
		tar_next->prev = NULL;
		target->next = NULL;
		free(target);
	}

	// element in middle
	else if(target->prev != NULL && target->next != NULL) {
		tar_prev = target->prev;
		tar_next = target->next;

		tar_prev->next = tar_next;
		tar_next->prev = tar_prev;

		target->prev = target->next = NULL;
		free(target);
	}

	else {
		tar_next = target->next;
		
		tar_next->next = NULL;
		target->prev = NULL;
		free(target);
	}		

}
