#include "linked_list.h"

// Destructor
List::~List(){
	// Delete Linked list
	Node* cur = head;
	while (cur != NULL) {
		Node* next = cur->pNext;
		delete cur;
		cur = next;
	}
	head = tail = NULL;
	size = 0;
}

// Copy constructor 
List::List(const List& l) : head(NULL), tail(NULL), size(0) {
	Node* p = l.head;
	while (p != NULL) {
		push_back(p->data);
		p = p->pNext;
	}
}

Node* List::createNode(const int& data) {
	// Create node
	Node* p = new Node;
	if (p == NULL)
		return NULL;
	p->data = data;
	p->pNext = NULL;
	return p;
}

void List::push_back(const int& data)
{
	Node* p = createNode(data);
	if (!head) {
		head = tail = p;
	}
	else {
		tail->pNext = p;
		tail = tail->pNext;
	}
	size++;
}

Node* List::get_head() const {
	return head;
}

int List::get_size() const {
	return size;
}

void List::decreaseSize() {
	size--;
}

void List::increaseSize() {
	size++;
}