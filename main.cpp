#include <iostream>
#include "graph.h"

int main(){
	AdjencyList g("Euler_circuit.txt");
	g.printGraph();
	AdjencyList copy(g);
	Hamilton h(&copy);
	h.run(0);
	/*Euler test(&copy);
	test.Euler_Print();*/
	/*AdjencyMatrix test("test1.txt");
	test.printGraph();
	shortestPath sol;
	sol.Dijkstra(test, 2);
	sol.Bellman_Ford(test, 0);*/
	return 0;
}