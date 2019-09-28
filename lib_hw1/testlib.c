#include "bitmap.h"
#include "hash.h"
#include "list.h"
#include "testlib.h"
#include "test_list.h"

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
	
	// variables for implementing commands
	struct ds_table *target = NULL;

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
			create_list(tokenize[2]);
		}
	}

	else if(!strcmp(command, "delete")){
		command_delete(tokenize[1]);
	}

	else if(!strcmp(command, "dumpdata")) {
		command_dumpdata(tokenize[1]);
	}

	//list_insert.in test
	else if(!strcmp(command, "list_insert")) {
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

	//list_frontback.in test + list_pop.in
	else if(!strcmp(command, "list_push_back")) {
		int data = atoi(tokenize[2]);

		// find the name data structure
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		// new data to insert
		struct list_item *new = malloc(sizeof(struct list_item));

		new->data = data;
		list_push_back(target->list, &(new->elem));	
	}
	else if(!strcmp(command, "list_push_front")) {
		int data = atoi(tokenize[2]);

		// find the name data structure
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		// new data to insert
		struct list_item *new = malloc(sizeof(struct list_item));

		new->data = data;
		list_push_front(target->list, &(new->elem));	
	}

	else if(!strcmp(command, "list_front")) {
		// find the name data structure
		target = find_ds_table(tokenize[1]);
		if(!target) return;
		
		if(!list_empty(target->list)) {
			struct list_elem* front;
			struct list_item* fr_item;

			front = list_front(target->list);
			if(!front) return;

			fr_item = list_entry(front, struct list_item, elem);
			printf("%d\n", fr_item->data);
		}
	}
	else if(!strcmp(command, "list_back")) {
		// find the name data structure
		target = find_ds_table(tokenize[1]);
		if(!target) return;
		
		if(!list_empty(target->list)) {
			struct list_elem* back;
			struct list_item* b_item;

			back = list_back(target->list);
			if(!back) return;

			b_item = list_entry(back, struct list_item, elem);
			printf("%d\n", b_item->data);
		}
	}
	else if(!strcmp(command, "list_pop_front")) {
		// find the name data structure
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		if(!list_empty(target->list)) {
			struct list_elem *e = list_pop_front(target->list);
			struct list_item *tar_item;

			tar_item = list_entry(e, struct list_item, elem);
			free(tar_item);
		}

	}
	else if(!strcmp(command, "list_pop_back")) {
		// find the name data structure
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		if(!list_empty(target->list)) {
			struct list_elem *e = list_pop_back(target->list);
			struct list_item *tar_item;

			tar_item = list_entry(e, struct list_item, elem);
			free(tar_item);
		}

	}
	
	// list_insert_ordered.in
	/*
	else if(!strcmp(command, "list_insert_ordered")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
		
	}
	*/
	
	// list_swap.in
	else if(!strcmp(command, "list_swap")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		int idx_a = atoi(tokenize[2]);
		int idx_b = atoi(tokenize[3]);

		// find the name data structure
		target = find_ds_table(tokenize[1]);
		if(!target) return;
		
		if(!list_empty(target->list)) {
			struct list_elem *a, *b;
			a = b = list_head(target->list);

			for(int i = 0; i <= idx_a; i++) 
				a = list_next(a);
			for(int i = 0; i <= idx_b; i++) 
				b = list_next(b);
			list_swap(a, b);
		}
	}
	
	// list_remove.in
	else if(!strcmp(command, "list_remove")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		int idx = atoi(tokenize[2]);

		if(!list_empty(target->list)) {
			struct list_elem *eptr = list_head(target->list);
			struct list_item *iptr;
			for(int i = 0; i <= idx; i++) {
				eptr = list_next(eptr);
			}
			iptr = list_entry(eptr, struct list_item, elem);
			list_remove(eptr);
			free(iptr);
		}
	}
	
	// list_reverse.in
	else if(!strcmp(command, "list_reverse")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
		
		list_reverse(target->list);
	}
	
	// list_splice.in
	else if(!strcmp(command, "list_splice")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
		
		struct ds_table* t2 = find_ds_table(tokenize[3]);
		int idx1, idx2, idx3;
		idx1 = atoi(tokenize[2]);
		idx2 = atoi(tokenize[4]);
		idx3 = atoi(tokenize[5]);

		struct list_elem *ptr1, *ptr2, *ptr3;
		ptr1 = list_head(target->list);
		ptr2 = ptr3 = list_head(t2->list);

		for(int i = 0; i <= idx1; i++) 
			ptr1 = list_next(ptr1);
		for(int i = 0; i <= idx2; i++) 
			ptr2 = list_next(ptr2);
		for(int i = 0; i <= idx3; i++) 
			ptr3 = list_next(ptr3);

		list_splice(ptr1, ptr2, ptr3);
	}

	// list_unique.in
	else if(!strcmp(command, "list_unique")) {
		target = find_ds_table(tokenize[1]);
		struct ds_table* t2 = find_ds_table(tokenize[2]);

		if(!target) return;
		if(!t2) t2 = find_ds_table(tokenize[1]);

		if(word_num == 3)
			list_unique(target->list, t2->list, less_value, NULL);
		else if(word_num == 2)
			list_unique(target->list, NULL, less_value, NULL);
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

void command_delete(char *name) {
	struct ds_table *target = NULL;

	target = find_ds_table(name);
	if(target == NULL) return;

	switch(target->type) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			delete_list(target);
			break;
	}
}

void delete_list(struct ds_table* target) {
	struct ds_table *tar_prev, *tar_next;
	struct list_item *tar_item;

	// free elements of list
	while(!list_empty(target->list)) {
		struct list_elem *e = list_pop_front(target->list);
		
		tar_item = list_entry(e, struct list_item, elem);
		free(tar_item);
	}

	//free list
	free(target->list);

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
