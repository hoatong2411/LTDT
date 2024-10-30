#pragma once
#include <iostream>
typedef struct Node {
	int data;
	Node* pNext;
} Node;

typedef struct List {
	Node* head;
	Node* tail;
	int szList;
} List;

void freeList(List& lst);
void initList(List& lst);
Node* createNode(const int& data);
bool insertHead(List& lst, const int& data);
bool insertTail(List& lst, const int& data);
