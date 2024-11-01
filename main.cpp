#include <stdio.h>
#include "graph.h"

int main(){
	// Euler test
	char Euler_graph[100];
	printf("Nhap vao ten tap tin chua do thi Euler: ");
	gets_s(Euler_graph);
	printf("Bai toan duong di/ chu trinh Euler cho do thi sau:\n");
	AdjacencyList a(Euler_graph);
	AdjacencyList copy1(a);
	Euler e(&copy1);
	e.Euler_Print();

	getchar(); // Ignore '\n' character
	// Hamilton test
	char Ham_graph[100];
	printf("Nhap vao ten tap tin chua do thi Hamilton: ");
	gets_s(Ham_graph);
	printf("Bai toan duong di/ chu trinh Hamilton cho do thi sau:\n");
	AdjacencyList b(Ham_graph);
	AdjacencyList copy2(b);
	Hamilton h(&copy2);
	h.Ham_print();
	return 0;
}