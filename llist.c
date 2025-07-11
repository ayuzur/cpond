#include "llist.h"
#include "utils.h"

void llist_add(Llist* list, void* data) {
	llist_Node* node = (llist_Node*) checkedCalloc(1, sizeof(llist_Node));

	node->data = data;
	node->next = NULL;

	if (list->head == NULL) {
		list->head = node;
		list->last = list->head;
		return;
	}

	list->last->next = node;
	list->last = node;
	
}

void llist_node_free(llist_Node* node) {
	if (node == NULL) {
		return;
	}

	llist_Node* next = node->next;
	free(node);
	llist_node_free(next);
}

void llist_free(Llist* list) {
	llist_node_free(list->head);
	free(list);
}
