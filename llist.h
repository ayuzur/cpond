#ifndef LLIST_H
#define LLIST_H

// linked list node
typedef struct llist_Node {
	void* data;
	struct llist_Node* next;
} llist_Node;

// linked list container struct
typedef struct Llist {
	llist_Node* head;
	llist_Node* last;
} Llist;

// add given data to list. this allocates a node
void llist_add(Llist* list, void* data);

// NOTE you should free contained data before using the following functions
// TODO maybe take a function pointer to a freeing function to free the data?
void llist_node_free(llist_Node* node);
void llist_free(Llist* list);


// TODO remove: i think this is unused
#define llist_iterate(list) for (llist_Node* node = list.head; node != NULL; node = node->next)

#endif
