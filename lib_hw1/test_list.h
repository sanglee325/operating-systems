#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void list_swap(struct list_elem *a, struct list_elem *b);
bool less_value (const struct list_elem *a, const struct list_elem *b, void *aux);
