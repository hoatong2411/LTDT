#pragma once

#include <iostream>
#include <vector>
#include "linked_list.h"
#include <limits.h>

typedef struct {
	int u, v;
	// u_pos , v_pos used to save the position of u (in linked list of v) 
	// and v (in linked list of u)
	int u_pos, v_pos;
} Edge;

class AdjacencyList {
private:
	std::vector<List*> lst;
	int nVer;
	friend class Euler;
	friend class Hamilton;

public:
	friend void DFS_visit(AdjacencyList* g, int ver, bool* label);
	// Constructor & Destructor
	AdjacencyList(const char* filename);
	~AdjacencyList();

	// Copy constructor
	AdjacencyList(const AdjacencyList& org);

	// Function to load graph from file
	void LoadGraph(const char* filename);

	// Print Graph
	void printGraph() const;

	// Define operator [] to access List[i]
	List* operator[](int index);
};

class Euler {
private:
	AdjacencyList* Graph;
	int startVer;
	// EulerType = 2 (circuit); = 1 (path); = 0 (none)
	int EulerType; 

	// Algorithm
	bool isConnected();
	int checkEulerGraph(int startVer);
	int DFS_Count(int ver, bool* label);
	bool isBrigde(int u, int v);
public:
	// Constructor
	Euler(AdjacencyList* g);
	bool hasCircuit() const;
	bool hasPath() const;

	// Operator with Edge
	Edge removeEdge(int u, int v);
	void addEdge(int u, int v, Edge e);

	// Print Result
	void Euler_Rec(int ver);
	void Euler_Print();
};

class Hamilton {
private:
	AdjacencyList* Graph;
	int startVer;
	// HamType = 2 (circuit); = 1 (path); = 0 (none)
	int HamType;

	// Store Circuit or Path of a Graph
	std::vector<std::vector<int>> circuit;
	std::vector<std::vector<int>> path;
	std::vector<int> curPath;

	// Algorithm
	bool isConnected();
	void Hamilton_Run();
	void Hamiton_Rec(int pos, int val);
public:
	// Constructor
	Hamilton(AdjacencyList* g);
	bool hasCircuit() const;
	bool hasPath() const;

	// Operator with Edge
	Edge removeEdge(int u, int v);
	void addEdge(int u, int v, Edge e);
	bool isEdge(int u, int v) const;

	// Result
	void Ham_print();
};

void DFS_visit(AdjacencyList* g, int ver, bool* label);
