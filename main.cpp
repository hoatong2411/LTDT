#include <iostream>
#include "graph.h"

int main(){
	// Euler test
	printf("Bai toan duong di/ chu trinh Euler cho do thi sau:\n");
	AdjencyList a("Euler_path.txt");
	AdjencyList copy1(a);
	Euler e(&copy1);
	e.Euler_Print();

	// Hamilton test
	printf("Bai toan duong di/ chu trinh Hamilton cho do thi sau:\n");
	AdjencyList b("hamilton_path.txt");
	AdjencyList copy2(b);
	Hamilton h(&copy2);
	h.Ham_print();
	/*Euler test(&copy);
	test.Euler_Print();*/
	/*AdjencyMatrix test("test1.txt");
	test.printGraph();
	shortestPath sol;
	sol.Dijkstra(test, 2);
	sol.Bellman_Ford(test, 0);*/
	return 0;
}