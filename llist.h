#ifndef LLIST_H
#define LLIST_H

typedef struct llist_Node {
	void* data;
	struct llist_Node* next;
} llist_Node;

typedef struct Llist {
	llist_Node* head;
	llist_Node* last;
} Llist;

void llist_add(Llist* list, void* data);

#define llist_iterate(list) for (llist_Node* node = list.head; node != NULL; node = node->next)

#endif
