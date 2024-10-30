#include "linked_list.h"

void freeList(List& lst)
{
	if (lst.head != NULL) {
		Node* p = lst.head, * temp = NULL;
		while (p != NULL) {
			temp = p->pNext;
			delete p;
			p = temp;
		}
		lst.head = NULL;
		lst.tail = NULL;
		lst.szList = 0;
	}
}

void initList(List& lst) {
	lst.head = NULL;
	lst.tail = NULL;
	lst.szList = 0;
}

Node* createNode(const int& data) {
	// Create node
	Node* p = new Node;
	if (p == NULL)
		return NULL;
	p->data = data;
	p->pNext = NULL;
	return p;
}

bool insertHead(List& lst, const int& data)
{
	// Create node
	Node* p = createNode(data);
	if (p == NULL) {
		return false;
	}
	// Modify list
	if (lst.head == NULL) {
		lst.head = lst.tail = p;
	}
	else {
		p->pNext = lst.head;
		lst.head = p;
	}
	lst.szList++;
	return true;
}

bool insertTail(List& lst, const int& data)
{
	Node* p = createNode(data);
	if (p == NULL) {
		return false;
	}
	if (lst.head == NULL) {
		lst.head = lst.tail = p;
	}
	else {
		lst.tail->pNext = p;
		lst.tail = lst.tail->pNext;
	}
	lst.szList++;
	return true;
}