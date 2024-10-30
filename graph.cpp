#include "graph.h"

// AdjencyList
void AdjencyList::ParseLine(char* line, List*& lst) {
	char* context = nullptr;
	char* p = strtok_s(line, " ", &context);
	int count = atoi(p), num;
	while ((p = strtok_s(NULL, " ", &context)) != NULL) {
		num = atoi(p);
		insertTail(*lst, num);
	}
}

AdjencyList::AdjencyList(const char* filename) {
	// Load graph
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, filename, "r");
	if (err != 0) {
		exit(1);
	}
	// Read nVer
	fscanf_s(fp, "%d\n", &nVer);
	lst.resize(nVer);
	char line[200];
	int i = 0;
	while (fgets(line, sizeof(line), fp)) {
		lst[i] = new List();
		ParseLine(line, lst[i]);
		i++;
	}
	fclose(fp);
}

// Copy constructor
AdjencyList::AdjencyList(AdjencyList& org) {
	nVer = org.nVer;
	lst.resize(nVer);
	for (int i = 0; i < nVer; i++) {
		lst[i] = new List();
		Node* p = org.lst[i]->head;
		while (p != NULL) {
			insertTail(*lst[i], p->data);
			p = p->pNext;
		}
	}
}

AdjencyList::~AdjencyList() {
	for (int i = 0; i < nVer; i++) {
		freeList(*lst[i]);
	}
}

void AdjencyList::printGraph() {
	printf("nVer: %d\n", nVer);
	for (int i = 0; i < nVer; i++) {
		printf("Point ");
		Node* p = lst[i]->head;
		while (p) {
			printf("%d ", p->data);
			p = p->pNext;
		}
		printf("\t Number of vertexs: %d\n", lst[i]->szList);
	}
}

// Remove and Add Edge function
void Euler::removeEdge(int u, int v){
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return;
	}

	// Remove ver v from adjencyList of u
	Node* p1 = Graph->lst[u]->head;
	while (p1 != NULL) {
		if (p1->data == v) {
			p1->data = -1;
			break;
		}
		p1 = p1->pNext;
	}
	Graph->lst[u]->szList--;

	// Remove ver u from adjencyList of v
	Node* p2 = Graph->lst[v]->head;
	while (p2 != NULL) {
		if (p2->data == u) {
			p2->data = -1;
			break;
		}
		p2 = p2->pNext;
	}
	Graph->lst[v]->szList--;
}

void Euler::addEdge(int u, int v) {
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return;
	}

	// Add ver v to adjencyList of u
	Node* p1 = Graph->lst[u]->head;
	while (p1 != NULL) {
		if (p1->data == -1) {
			p1->data = v;
			break;
		}
		p1 = p1->pNext;
	}
	Graph->lst[u]->szList++;

	// Add ver u to adjencyList of v
	Node* p2 = Graph->lst[v]->head;
	while (p2 != NULL) {
		if (p2->data == -1) {
			p2->data = u;
			break;
		}
		p2 = p2->pNext;
	}
	Graph->lst[v]->szList++;
}

// DFS
void Euler::DFS_visit(int ver, bool* label) {
	label[ver] = true;
	Node* p = Graph->lst[ver]->head;
	while (p != NULL) {
		if (label[p->data] == false) {
			DFS_visit(p->data, label);
		}
		p = p->pNext;
	}
}

bool Euler::isConnected() {
	bool* label = new bool[Graph->nVer];
	memset(label, false, Graph->nVer * sizeof(bool));
	int startVer = -1; 
	for (int i = 0; i < Graph->nVer; i++) {
		if (Graph->lst[i]->szList != 0) {
			startVer = i;
			break;
		}
	}
	if (startVer == -1) {
		delete[]label;
		return true;
	}
	bool connect = true;
	DFS_visit(startVer, label);
	for (int j = 0; j < Graph->nVer; j++) {
		if (label[j] == false && Graph->lst[j]->szList != 0) {
			connect = false;
			break;
		}
	}
	delete[]label;
	return connect;
}

// Euler path/circuit
int Euler::DFS_Count(int ver, bool* label) {
	if (ver < 0 || ver >= Graph->nVer) {
		return 0;
	}

	int count = 1;
	label[ver] = true;
	Node* p = Graph->lst[ver]->head;
	while (p != NULL) {
		if (p->data >= 0 && label[p->data] == false) {
			count += DFS_Count(p->data, label);
		}
		p = p->pNext;
	}
	return count;
}

int Euler::isEuler(int startVer) {
	if (isConnected() == false) {
		return 0;
	}
	int odd = 0;
	for (int i = 0; i < Graph->nVer; i++) {
		if (Graph->lst[i]->szList % 2 == 1) {
			odd++;
		}
	}
	if (odd > 2 || (odd == 2 && Graph->lst[startVer]->szList%2 == 0)) {
		return 0;
	}
	else {
		return (odd == 0) ? 2 : 1;
	}
}

bool Euler::isBrigde(int u, int v) {
	bool* label = new bool[Graph->nVer];
	memset(label, false, Graph->nVer * sizeof(bool));
	// Count vertex reachable from u before remove edge {u,v}
	int countBefore = DFS_Count(u, label);
	
	// Count vertex reachable from u before remove edge {u,v}
	memset(label, false, Graph->nVer * sizeof(bool));
	removeEdge(u, v);
	int countAfter = DFS_Count(u, label);

	// Restore {u,v}
	addEdge(u, v);
	delete[]label;

	return (countBefore > countAfter);
}

void Euler::Euler_Rec(int ver) {
	if (ver < 0 || ver >= Graph->nVer || Graph->lst[ver]->szList == 0) {
		// printf("End at ver %d\n", ver);
		return;
	}
	Node* p = Graph->lst[ver]->head;
	while (p != NULL) {
		// printf("DEBUG VER %d at val: %d\n", ver, p->data);
		if (p->data >= 0) {
			int u = ver, v = p->data;
			if (Graph->lst[u]->szList == 1 || !isBrigde(u, v)) {
				printf("%d -> %d \n",u, v);
				removeEdge(u, v);
				Euler_Rec(v);
			}
		}
		p = p->pNext;
	}
}

void Euler::Euler_Print() {
	printf("Nhap dinh bat dau: ");
	int startVer;
	scanf_s("%d", &startVer);
	int isEuler_val = isEuler(startVer);
	if (isEuler_val > 0) {
		if (isEuler_val == 1) {
			printf("Duong di Euler:\n");
		}
		else {
			printf("Chu trinh Euler:\n");
		}
		Euler_Rec(startVer);
	}
	else {
		printf("Do thi khong co duong di hay chu trinh Euler\n");
	}
	return;
}

Hamilton::Hamilton(AdjencyList* g) : Graph(g) {
	circuit = new int[Graph->nVer];
	memset(circuit, 0, Graph->nVer * sizeof(int));
}

Hamilton::~Hamilton() {
	delete[]circuit;
	circuit = NULL;
}

bool Hamilton::isEdge(int u, int v) {
	Node* p = Graph->lst[u]->head;
	while (p != NULL) {
		if (p->data == v) {
			return true;
		}
		p = p->pNext;
	}
	return false;
}

void Hamilton::run(int ver) {
	// Algorithm: https://www.youtube.com/watch?v=dQr4wZCiJJ4

	printf("Hamilton run at %d", ver);
	do {
		NextVertex(ver);
		if (circuit[ver] == -1) {
			// No vertexs found
			return;
		}
		if (ver == Graph->nVer - 1) {
			// last vertexs forms a cycle with first vertexs
			if (isEdge(circuit[0], circuit[ver])) {
				printf("%d ", circuit[0]);
				for (int i = 1; i < Graph->nVer; i++) {
					printf("-> %d ", circuit[i]);
				}
				printf("\n");
			}
		}
		else {
			printf("\tValue: %d\n", circuit[ver]);
			run(ver + 1);
		}
	} while (true);
}

void Hamilton::NextVertex(int ver) {
	do {
		// Start vertex is 0, so need to minus 1 after taking modular
		circuit[ver] = (circuit[ver] + 1) % Graph->nVer;

		// No vertexs found
		if (circuit[ver] == -1) {
			return;
		}

		// Vertex 0 have no previous vertex in circuit array
		if (ver == 0) {
			return;
		}
		if (isEdge(circuit[ver - 1], circuit[ver])) {
			int i = 0;
			for (; i < ver; i++) {
				if (circuit[i] == circuit[ver]) {
					break;
				}
			}
			if (i == ver) {
				if (ver < Graph->nVer - 1 || ((ver == Graph->nVer - 1) && isEdge(circuit[0], circuit[ver]))){
					return;
				}
			}
		}
	} while (true);
}

// AjdencyMatrix
AdjencyMatrix::AdjencyMatrix(const char* filename) {
		FILE* fp = NULL;
		errno_t err = fopen_s(&fp, filename, "r");
		if (err != 0) {
			exit(1);
		}
		fscanf_s(fp, "%d\n", &nVer);
		data.resize(nVer);
		for (int i = 0; i < nVer; i++) {
			data[i].resize(nVer);
			for (int j = 0; j < nVer; j++) {
				fscanf_s(fp, "%d", &data[i][j]);
			}
		}
		fclose(fp);
}

void AdjencyMatrix::printGraph() {
	for (int i = 0; i < nVer; i++) {
		for (int j = 0; j < nVer; j++) {
			printf("%3d", data[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

// ShortestPath
void shortestPath::Dijkstra(AdjencyMatrix& a, int source) {
	// Label
	bool* lb = new bool[a.nVer];
	memset(lb, 1, a.nVer);
	// Length
	int* len = new int[a.nVer];
	for (int i = 0; i < a.nVer; i++) {
		len[i] = INT32_MAX;
	}
	len[source] = 0;

	// Prev
	int* prev = new int[a.nVer];
	for (int i = 0; i < a.nVer; i++) {
		prev[i] = -1;
	}
	prev[source] = 0;

	// Run
	int n = a.nVer;
	bool find = true;
	while (n > 0 && find) {
		// Find min L[i]
		int minL = INT32_MAX, ver = -1;
		for (int i = 0; i < a.nVer; i++) {
			if (lb[i] == true && len[i] < minL) {
				minL = len[i];
				ver = i;
			}
		}
		// Check
		if (ver == -1) {
			find = false;
			break;
		}
		// Update L[j]
		for (int i = 0; i < a.nVer; i++) {
			if (a.data[ver][i] != 0 && len[i] > (len[ver] + a.data[ver][i])) {
				prev[i] = ver;
				len[i] = len[ver] + a.data[ver][i];
			}
		}
		lb[ver] = false;
		n--;
	}

	// Print result
	for (int i = 0; i < a.nVer; i++) {
		if (lb[i] == false && i != source) {
			printf("Duong di tu dinh %d den dinh %d: %d <- ", source + 1, i + 1, i + 1);
			int ver = prev[i];
			while (ver != source) {
				printf("%d <- ", ver + 1);
				ver = prev[ver];
			}
			printf("%d\n", ver + 1);
		}
	}

	printf("\n");
	for (int i = 0; i < a.nVer; i++) {
		if (i != source) {
			printf("Ver %d - Len: %d\n", i, len[i]);
		}
	}
	printf("\n");
	delete[]lb;
	delete[]len;
	delete[]prev;
}

void shortestPath::Bellman_Ford(AdjencyMatrix& a, int source) {
	std::vector<std::vector<int>> cost;
	std::vector<std::vector<int>> prev;
	cost.resize(a.nVer + 1);
	prev.resize(a.nVer + 1);
	for (int step = 0; step < a.nVer + 1; step++) {
		cost[step].resize(a.nVer);
		prev[step].resize(a.nVer);
	}

	// Khoi tao gia tri
	for (int i = 0; i < a.nVer; i++) {
		cost[0][i] = INT32_MAX;
		prev[0][i] = -1;
	}
	cost[0][source] = 0;

	// Run
	for (int j = 0; j < a.nVer; j++) {
		printf("%11d", cost[0][j]);
	}
	printf("\n");
	int step = 1;
	for (; step <= a.nVer; step++) {
		for (int i = 0; i < a.nVer; i++) {
			cost[step][i] = cost[step - 1][i];
			prev[step][i] = prev[step - 1][i];
		}
		// Update cost[step][i]
		for (int sr = 0; sr < a.nVer; sr++) {
			// Neu da co duong di tu source den sr
			if (cost[step - 1][sr] != INT32_MAX) {
				for (int dst = 0; dst < a.nVer; dst++) {
					if (a.data[sr][dst] != 0 &&
						cost[step][dst] > (cost[step - 1][sr] + a.data[sr][dst])) {
						cost[step][dst] = cost[step - 1][sr] + a.data[sr][dst];
						prev[step][dst] = sr;
					}
				}
			}
		}
		for (int j = 0; j < a.nVer; j++) {
			printf("%11d", cost[step][j]);
		}
		printf("\n");

		// Check 
		int i = 0;
		for (; i < a.nVer && cost[step][i] == cost[step - 1][i]; i++);
		if (i == a.nVer) {
			break;
		}
	}

	if (step == (a.nVer + 1)) {
		printf("Ton tai mach am trong do thi\n");
	}
	else {
		for (int i = 0; i < a.nVer; i++) {
			if (cost[step][i] != INT32_MAX && cost[step][i] != 0) {
				printf("Duong di tu dinh %d den dinh %d: %d <- ", source + 1, i + 1, i + 1);
				int ver = prev[step][i];
				while (ver != source) {
					printf("%d <- ", ver + 1);
					ver = prev[step][ver];
				}
				printf("%d\n", ver + 1);
			}
		}
	}
}
