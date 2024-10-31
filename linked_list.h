#pragma once
#include <iostream>
typedef struct Node {
	int data;
	Node* pNext;
} Node;

typedef class List {
	Node* head;
	Node* tail;
	int size;
	friend class AdjencyList;
public:
	// Constructor and destructor
	List() : head(NULL), tail(NULL), size(0) {};
	~List();

	// Copy constructor
	List(const List& l);

	// Add node function
	Node* createNode(const int& data);
	void push_back(const int& data);
	
	Node* get_head() const;
	int get_size() const;
	// Temporary size decreasing and increasing Function
	void decreaseSize();
	void increaseSize();
} List;
