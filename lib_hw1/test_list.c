#include "testlib.h"
#include "test_list.h"

void list_swap(struct list_elem *a, struct list_elem *b) {
	struct list_elem* before_a = list_prev(a);
	struct list_elem* next_a = list_next(a);
	struct list_elem* before_b = list_prev(b);
	struct list_elem* next_b = list_next(b);

	// disconnect others with a and b, and connect to b and a
	before_a->next = next_a->prev = b;
	before_b->next = next_b->prev = a;

	struct list_elem* swap_ab;
	swap_ab = a->prev;
	a->prev = b->prev;
	b->prev = swap_ab;

	swap_ab = a->next;
	a->next = b->next;
	b->next = swap_ab;
}

void list_shuffle(struct list* list) {
	int times, idx1, idx2;
	int size = list_size(list);

	srand(time(NULL));

	times = rand()%20 + 5;

	for(int i = 0; i < times; i++){
		idx1 = rand()%size;
		idx2 = rand()%size;
		// printf("idx1: %d idx2: %d\n", idx1, idx2);

		struct list_elem* e1 = list_begin(list);
		struct list_elem* e2 = list_begin(list);
		for(int j = 0; j < idx1; j++)
			e1 = list_next(e1);
		for(int j = 0; j < idx2; j++)
			e2 = list_next(e2);
		list_swap(e1, e2);
	}
}

bool less_value_list (const struct list_elem *a, const struct list_elem *b,
             void *aux) {
	struct list_item *item1 = list_entry(a, struct list_item, elem);
	struct list_item *item2 = list_entry(b, struct list_item, elem);

	return item1->data < item2->data;
}

void command_list(char *command, char tokenize[MAX_INPUT_LEN][MAX_INPUT_LEN], int word_num) {
	struct ds_table *target = NULL;

	//list_insert.in test
	if(!strcmp(command, "list_insert")) {
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
	else if(!strcmp(command, "list_insert_ordered")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		int data = atoi(tokenize[2]);

		struct list_item* new = malloc(sizeof(struct list_item));
		new->data = data;
		
		list_insert_ordered(target->list, &(new->elem), less_value_list, NULL);
	}
	
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
			list_unique(target->list, t2->list, less_value_list, NULL);
		else if(word_num == 2)
			list_unique(target->list, NULL, less_value_list, NULL);
	}
	else if(!strcmp(command, "list_sort")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
	
		list_sort(target->list, less_value_list, NULL);
	}

	else if(!strcmp(command, "list_max")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
	
		struct list_elem *max_elem = list_max(target->list, less_value_list, NULL);
		struct list_item *max_item = list_entry(max_elem, struct list_item, elem);
		printf("%d\n", max_item->data);
	}

	else if(!strcmp(command, "list_min")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
	
		struct list_elem *min_elem = list_min(target->list, less_value_list, NULL);
		struct list_item *min_item = list_entry(min_elem, struct list_item, elem);
		printf("%d\n", min_item->data);
	}

	else if(!strcmp(command, "list_empty")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
	
		if(list_empty(target->list)) printf("true\n");
		else printf("false\n");
	}

	else if(!strcmp(command, "list_size")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
		
		int size = list_size(target->list);
		printf("%d\n", size);
	}

	else if(!strcmp(command, "list_shuffle")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;
		
		list_shuffle(target->list);
	}
}
