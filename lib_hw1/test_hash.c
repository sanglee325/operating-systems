#include "testlib.h"
#include "test_hash.h"

bool less_value_hash (const struct hash_elem *a, const struct hash_elem *b, void *aux) {
	unsigned x = hash_value_elem(a, aux);
	unsigned y = hash_value_elem(b, aux);

	return x < y;
}

unsigned hash_value_elem(const struct hash_elem *e, void *aux) {
	struct hash_item *item = hash_entry(e, struct hash_item, elem);
	return hash_int(item->data); 
}

void h_hash_free(struct hash_elem *e, void *aux) {
	struct hash_item* item = hash_entry(e, struct hash_item, elem);
	free(item);
}

void hash_act_sq(struct hash_elem *a,void *aux){
	struct hash_item *item = hash_entry(a, struct hash_item ,elem);
	item->data = item->data * item->data;
}

void hash_act_tri(struct hash_elem *a,void *aux){
	struct hash_item *item = hash_entry(a, struct hash_item ,elem);
	item->data = item->data * item->data * item->data;
}

void command_hash(char *command, char tokenize[MAX_INPUT_LEN][MAX_INPUT_LEN], int word_num) {
	struct ds_table *target = NULL;

	if(!strcmp(command, "hash_insert")) {
		int data = atoi(tokenize[2]);

		target = find_ds_table(tokenize[1]);
		if(!target) return;

		struct hash_item *new = malloc(sizeof(struct hash_item));
		new->data = data;

		hash_insert(target->hash, &(new->elem));
	}

	else if(!strcmp(command, "hash_delete")) {
		int data = atoi(tokenize[2]);

		target = find_ds_table(tokenize[1]);
		if(!target) return;

		struct hash_item *new = malloc(sizeof(struct hash_item));
		new->data = data;

		hash_delete(target->hash, &(new->elem));
	}

	else if(!strcmp(command, "hash_apply")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		if(!strcmp(tokenize[2], "square")) {
			hash_apply(target->hash, &hash_act_sq);
		}
		else if(!strcmp(tokenize[2], "triple")) {
			hash_apply(target->hash, &hash_act_tri);
		}
	}

	else if(!strcmp(command, "hash_empty")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		if(hash_empty(target->hash)) {
			printf("true\n");
		}
		else {
			printf("false\n");
		}
	}

	else if(!strcmp(command, "hash_size")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		int cnt = hash_size(target->hash);
		printf("%d\n", cnt);
	}

	else if(!strcmp(command, "hash_clear")) {
		target = find_ds_table(tokenize[1]);
		if(!target) return;

		hash_clear(target->hash, NULL);
	}
}
