#include "list.h"
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

bool less_value (const struct list_elem *a, const struct list_elem *b,
             void *aux) {
	struct list_item *item1 = list_entry(a, struct list_item, elem);
	struct list_item *item2 = list_entry(b, struct list_item, elem);

	return item1->data < item2->data;
}
