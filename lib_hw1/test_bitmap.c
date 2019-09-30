#include "testlib.h"
#include "test_bitmap.h"

void command_bitmap(char *command, char tokenize[MAX_INPUT_LEN][MAX_INPUT_LEN], int word_num) {
	struct ds_table *target = NULL;

	if(!strcmp(command, "bitmap_mark")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int bit_idx = atoi(tokenize[2]);
		bitmap_mark(target->bitmap, bit_idx);
	}

	else if(!strcmp(command, "bitmap_all")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int start = atoi(tokenize[2]);
		int cnt = atoi(tokenize[3]);

		if(bitmap_all(target->bitmap, start, cnt)) printf("true\n");
		else printf("false\n");
	}
}
