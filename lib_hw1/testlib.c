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
	int i = 0;
	int idx = 0, word_num = 0, token_idx = 0;
	char *command;
	char tokenize[MAX_INPUT_LEN][MAX_INPUT_LEN] = { 0 };
	bool word[MAX_INPUT_LEN] = { false };

	for(i = 0; i < strlen(input_str); i++) {
		if(('!' <= input_str[i] && input_str[i] <= '~')) {
			word[i] = true;
		}
	}

	for(i = 1; i < strlen(input_str); i++) {
		if(word[i-1] == true && word[i] == false) {
			word_num++;
		}
	}

	if(word_num > 0) {
		idx = 0;
		for(i = 0; i < strlen(input_str); i++){
			if(input_str[i]) {
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

	if(!strcmp(command, "quit")){
		command_quit();
	}
	else if(!strcmp(command, "create")){
		if(!strcmp(tokenize[1], "bitmap")){
			create_bitmap(tokenize[2]);
		}
		else if(!strcmp(tokenize[1], "list")){
			create_list(tokenize[2]);
		}
		else if(!strcmp(tokenize[1], "hashtable")){
			create_list(tokenize[2]);
		}
	}

}

void command_quit() {
	if(list_tab_head) {
	}

	exit_flag = true;
}

void create_bitmap(char *name) {
	
}

void create_list(char *name) {
	struct list_table *new, *cur;

	new = (struct list_table*)malloc(sizeof(struct list_table));
	list_init(new->list);
	strcpy(new->name, name);
	new->next = NULL;

	if(!list_tab_head) {
		list_tab_head = new;
	}
	else {
		cur = list_tab_head;
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
