#pragma once

#include <iostream>
#include <vector>
#include "linked_list.h"
#include <limits.h>

typedef struct {
	int u, v;
	int u_pos, v_pos;
} Edge;

class AdjencyList {
private:
	std::vector<List*> lst;
	int nVer;
	friend class Euler;
	friend class Hamilton;
public:
	friend void DFS_visit(AdjencyList* g, int ver, bool* label);
	// Constructor & Destructor
	AdjencyList(const char* filename);
	~AdjencyList();

	// Copy constructor
	AdjencyList(AdjencyList& org);

	// Function to load graph from file
	void LoadGraph(const char* filename);

	// Print Graph
	void printGraph();

	// Define operator [] to access List[i]
	List* operator[](int index);
};

class Euler {
private:
	AdjencyList* Graph;
	int startVer;
	int EulerType; // EulerType = 2 (circuit); = 1 (path); = 0 (none)
	
public:
	bool hasCircuit() const;
	bool hasPath() const;
	// Constructor
	Euler(AdjencyList* g);

	// Operator with Edge
	Edge removeEdge(int u, int v);
	void addEdge(int u, int v, Edge e);

	// Algorithm
	bool isConnected();
	int checkEulerGraph(int startVer);
	int DFS_Count(int ver, bool* label);
	bool isBrigde(int u, int v);

	// Print Result
	void Euler_Rec(int ver);
	void Euler_Print();
};

class Hamilton {
private:
	AdjencyList* Graph;
	int startVer;

	// HamType = 2 (circuit); = 1 (path); = 0 (none)
	int HamType;

	// Store Circuit or Path of a Graph
	std::vector<std::vector<int>> circuit;
	std::vector<std::vector<int>> path;
	std::vector<int> curPath;
public:
	bool hasCircuit() const;
	bool hasPath() const;
	// Constructor and Destructor
	Hamilton(AdjencyList* g);
	~Hamilton();

	// Operator with Edge
	Edge removeEdge(int u, int v);
	void addEdge(int u, int v, Edge e);
	bool isEdge(int u, int v);

	// Algorithm
	bool isConnected();
	void run();
	void Hamiton_Rec(int pos, int val);

	// Result
	void Ham_print();
};

void DFS_visit(AdjencyList* g, int ver, bool* label);
