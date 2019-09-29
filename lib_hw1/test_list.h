#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "list.h"

void list_swap(struct list_elem *a, struct list_elem *b);
bool less_value_list (const struct list_elem *a, const struct list_elem *b, void *aux);
void command_list(char *command, char tokenize[MAX_INPUT_LEN][MAX_INPUT_LEN], int word_num);
