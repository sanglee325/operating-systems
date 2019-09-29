#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hash.h"

bool less_value_hash (const struct hash_elem *a, const struct hash_elem *b, void *aux);
unsigned hash_value_elem(const struct hash_elem *e, void *aux); 

void command_hash(char *command, char tokenize[MAX_INPUT_LEN][MAX_INPUT_LEN], int word_num);
void h_hash_free(struct hash_elem *e, void *aux);
