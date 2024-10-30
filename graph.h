#pragma once

#include <iostream>
#include <vector>
#include "linked_list.h"
#include <limits.h>
#include <list>

class AdjencyList {
private:
	int nVer;
	std::vector<List*> lst;
	friend class Euler;
	friend class Hamilton;
public:
	// Function to parse input
	void ParseLine(char* line, List*& lst);

	// Constructor & Destructor
	AdjencyList(const char* filename);
	~AdjencyList();

	// Copy constructor
	AdjencyList(AdjencyList& org);

	void printGraph();
};

class Euler {
	AdjencyList* Graph;
public:
	Euler(AdjencyList* g) : Graph(g){}
	void removeEdge(int u, int v);
	void addEdge(int u, int v);

	// Use DFS to check a graph is connected or not
	void DFS_visit(int ver, bool* label);
	bool isConnected();

	// Euler trail/circuit
	int isEuler(int startVer);
	int DFS_Count(int ver, bool* label);
	bool isBrigde(int u, int v);
	void Euler_Rec(int ver);
	void Euler_Print();
};

class Hamilton {
	AdjencyList* Graph;
	int* circuit;
public:
	// Constructor and Destructor
	Hamilton(AdjencyList* g);
	~Hamilton();

	bool isEdge(int u, int v);
	void run(int ver);
	void NextVertex(int ver);
};

class AdjencyMatrix {
	int nVer;
	std::vector<std::vector<int>> data;
	friend class shortestPath;
public:
	AdjencyMatrix(const char* filename);
	void printGraph();
};

class shortestPath {
public: 
	void Dijkstra(AdjencyMatrix& a, int source);
	void Bellman_Ford(AdjencyMatrix& a, int source);
};
