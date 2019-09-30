#include "testlib.h"
#include "test_bitmap.h"
#include "bitmap.h"


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
	
	else if(!strcmp(command, "bitmap_any")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int start = atoi(tokenize[2]);
		int cnt = atoi(tokenize[3]);

		if(bitmap_any(target->bitmap, start, cnt)) printf("true\n");
		else printf("false\n");
	}

	else if(!strcmp(command, "bitmap_contains")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int start = atoi(tokenize[2]);
		int cnt = atoi(tokenize[3]);

		bool value;
		if(!strcmp(tokenize[4], "true")) value = true;
		else if(!strcmp(tokenize[4], "false")) value = false;

		if(bitmap_contains(target->bitmap, start, cnt, value)) printf("true\n");
		else printf("false\n");
	}

	else if(!strcmp(command, "bitmap_count")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int start = atoi(tokenize[2]);
		int cnt = atoi(tokenize[3]);

		bool value;
		if(!strcmp(tokenize[4], "true")) value = true;
		else if(!strcmp(tokenize[4], "false")) value = false;

		int value_cnt = bitmap_count(target->bitmap, start, cnt, value);
		printf("%d\n", value_cnt);
	}

	else if(!strcmp(command, "bitmap_dump")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;
	
		bitmap_dump(target->bitmap);
	}

	else if(!strcmp(command, "bitmap_expand")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;
	
		bitmap_expand(target->bitmap, atoi(tokenize[2]));
	}

	else if(!strcmp(command, "bitmap_set_all")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;
	
		bool value;
		if(!strcmp(tokenize[2], "true")) value = true;
		else if(!strcmp(tokenize[2], "false")) value = false;

		bitmap_set_all(target->bitmap, value);
	}	

	else if(!strcmp(command, "bitmap_flip")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int bit_idx = atoi(tokenize[2]);
		bitmap_flip(target->bitmap, bit_idx);
	}
		
	else if(!strcmp(command, "bitmap_none")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int start = atoi(tokenize[2]);
		int cnt = atoi(tokenize[3]);

		if(bitmap_none(target->bitmap, start, cnt)) printf("true\n");
		else printf("false\n");
	}	

	else if(!strcmp(command, "bitmap_reset")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int bit_idx = atoi(tokenize[2]);
		bitmap_reset(target->bitmap, bit_idx);
	}

	else if(!strcmp(command, "bitmap_scan_and_flip")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int start = atoi(tokenize[2]);
		int cnt = atoi(tokenize[3]);

		bool value;
		if(!strcmp(tokenize[4], "true")) value = true;
		else if(!strcmp(tokenize[4], "false")) value = false;

		unsigned int idx = bitmap_scan_and_flip(target->bitmap, start, cnt, value);
		printf("%u\n", idx);
	}	

	else if(!strcmp(command, "bitmap_scan")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int start = atoi(tokenize[2]);
		int cnt = atoi(tokenize[3]);

		bool value;
		if(!strcmp(tokenize[4], "true")) value = true;
		else if(!strcmp(tokenize[4], "false")) value = false;

		unsigned int idx = bitmap_scan(target->bitmap, start, cnt, value);
		printf("%u\n", idx);
	}	

	else if(!strcmp(command, "bitmap_set")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;
	
		int idx = atoi(tokenize[2]);

		bool value;
		if(!strcmp(tokenize[3], "true")) value = true;
		else if(!strcmp(tokenize[3], "false")) value = false;

		bitmap_set(target->bitmap, idx, value);
	}	

	else if(!strcmp(command, "bitmap_set_multiple")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int start = atoi(tokenize[2]);
		int cnt = atoi(tokenize[3]);

		bool value;
		if(!strcmp(tokenize[4], "true")) value = true;
		else if(!strcmp(tokenize[4], "false")) value = false;

		bitmap_set_multiple(target->bitmap, start, cnt, value);
	}	

	else if(!strcmp(command, "bitmap_size")) {
		target = find_ds_table(tokenize[1]);

		int size = bitmap_size(target->bitmap);
		printf("%d\n", size);
	}
	
	else if(!strcmp(command, "bitmap_test")) {
		target = find_ds_table(tokenize[1]);
		if(target == NULL) return;

		int bit_idx = atoi(tokenize[2]);
		bool value = bitmap_test(target->bitmap, bit_idx);

		if(value) printf("true\n");
		else if(!value) printf("false\n");
	}			
}
