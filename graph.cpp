#include "graph.h"

// Constructor
AdjacencyList::AdjacencyList(const char* filename) {
	LoadGraph(filename);
}

// Destructor
AdjacencyList::~AdjacencyList() {
	for (int i = 0; i < nVer; i++) {
		// Delete entire List class lst[i]
		delete lst[i];
	}
}

// Copy constructor
AdjacencyList::AdjacencyList(AdjacencyList& org) : nVer(org.nVer) {
	lst.resize(nVer);
	for (int i = 0; i < nVer; i++) {
		lst[i] = new List(*org.lst[i]);
	}
}

// Function to to load graph from .txt file 
void AdjacencyList::LoadGraph(const char* filename) {
	FILE* fp = NULL;
	errno_t err = fopen_s(&fp, filename, "r");
	if (err != 0) {
		exit(1);
	}

	// Read nVer
	fscanf_s(fp, "%d\n", &nVer);
	lst.resize(nVer);
	for (int i = 0; i < nVer; i++) {
		lst[i] = new List();
	}

	// Parse input
	char line[200];
	int i = 0;
	while (fgets(line, sizeof(line), fp)) {
		char* context = nullptr;

		// Extract number of vertexs connect to this vertex
		char* p = strtok_s(line, " ", &context);

		// Extract these vertexs connect to this vertex
		while ((p = strtok_s(NULL, " ", &context)) != NULL) {
			lst[i]->push_back(atoi(p));
		}
		i++;
	}
	fclose(fp);
}


void AdjacencyList::printGraph() {
	printf("nVer: %d\n", nVer);
	for (int i = 0; i < nVer; i++) {
		printf("Point %d: ", i);
		Node* p = lst[i]->get_head();
		while (p != NULL) {
			printf("%d ", p->data);
			p = p->pNext;
		}
		printf("\t\tNumber of vertexs: %d\n", lst[i]->get_size());
	}
	printf("\n");
}

// Define operator [] to access List[i]
List* AdjacencyList::operator[](int index) {
	if (index < 0 || index >= nVer) {
		return NULL;
	}
	return lst[index];
}

///////////////////////////////////////////
// Euler class 

// Euler constructor
Euler::Euler(AdjacencyList* g) : Graph(g) {
	(*g).printGraph();
	printf("Nhap dinh bat dau: ");
	scanf_s("%d", &startVer);
	EulerType = checkEulerGraph(startVer);
}

bool Euler::hasCircuit() const {
	return (EulerType == 2);
}

bool Euler::hasPath() const {
	return (EulerType == 1);
}

// Temporary remove edge function
Edge Euler::removeEdge(int u, int v){
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return { -1, -1 ,0 , 0};
	}

	Edge e = { u, v , 0, 0 };

	// Remove ver v from adjencyList of u
	Node* p1 = (*Graph)[u]->get_head();
	while (p1 != NULL) {
		if (p1->data == v) {
			p1->data = -1;
			break;
		}
		e.v_pos++;
		p1 = p1->pNext;
	}
	(*Graph)[u]->decreaseSize();

	// Remove ver u from adjencyList of v
	Node* p2 = (*Graph)[v]->get_head();
	while (p2 != NULL) {
		if (p2->data == u) {
			p2->data = -1;
			break;
		}
		e.u_pos++;
		p2 = p2->pNext;
	}
	(*Graph)[v]->decreaseSize();

	return e;
}

// Temporary add edge function
void Euler::addEdge(int u, int v, Edge e) {
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return;
	}

	// Add ver v to adjencyList of u
	Node* p1 = (*Graph)[u]->get_head();
	while (p1 != NULL) {
		if (e.v_pos == 0 && p1->data == -1) {
			p1->data = v;
			break;
		}
		e.v_pos--;
		p1 = p1->pNext;
	}
	(*Graph)[u]->increaseSize();

	// Add ver u to adjencyList of v
	Node* p2 = (*Graph)[v]->get_head();
	while (p2 != NULL) {
		if (e.u_pos == 0 && p2->data == -1) {
			p2->data = u;
			break;
		}
		e.u_pos--;
		p2 = p2->pNext;
	}
	(*Graph)[v]->increaseSize();
}

// Using DFS to check a Graph is connected or not
// This algorithm is different with check a Hamilton graph is connected or not
// An Eulerian graph with isolated vertices can still be connected, 
// but a Hamiltonian graph cannot.
bool Euler::isConnected() {
	// Initialize an array to check that a vertex is visited or not
	bool* label = new bool[Graph->nVer];
	memset(label, false, Graph->nVer * sizeof(bool));

	// Find a non-isolated vertex to run DFS
	int DFS_startVer = -1;
	for (int i = 0; i < Graph->nVer; i++) {
		if ((*Graph)[i]->get_size() != 0) {
			DFS_startVer = i;
			break;
		}
	}

	// If all vertexs are isolated -> The graph is connected
	if (DFS_startVer == -1) {
		delete[]label;
		return true;
	}

	bool connect = true;
	DFS_visit(Graph, DFS_startVer, label);
	for (int j = 0; j < Graph->nVer; j++) {
		// If exists a vertex with positive degree but cannot be reachable -> not connected
		if (label[j] == false && (*Graph)[j]->get_size() != 0) {
			connect = false;
			break;
		}
	}

	delete[]label;
	return connect;
}

// DFS_Count function to determine the numbers of connected vertexs with vertex ver
int Euler::DFS_Count(int ver, bool* label) {
	if (ver < 0 || ver >= Graph->nVer) {
		return 0;
	}

	int count = 1;
	label[ver] = true;
	Node* p = (*Graph)[ver]->get_head();
	while (p != NULL) {
		if (p->data >= 0 && label[p->data] == false) {
			count += DFS_Count(p->data, label);
		}
		p = p->pNext;
	}
	return count;
}

// Return Euler Type of a graph
// - 0: a non-connected graph/ startVer is an isolated vertex/ a graph with 
// Euler path/circuit but startVer is not a odd-degree vertex
// - 1: a Graph with Euler path
// - 2: a Graph with Euler circuit
int Euler::checkEulerGraph(int startVer) {
	// In case startVer is an isolated vertex
	if (isConnected() == false || (*Graph)[startVer]->get_size() == 0) {
		return 0;
	}

	// Count odd-degree vertex
	int odd = 0;
	for (int i = 0; i < Graph->nVer; i++) {
		if ((*Graph)[i]->get_size() % 2 == 1) {
			odd++;
		}
	}

	if (odd > 2 || (odd == 2 && (*Graph)[startVer]->get_size() % 2 == 0)) {
		return 0;
	}
	else {
		return (odd == 0) ? 2 : 1;
	}
}

bool Euler::isBrigde(int u, int v) {
	// Label array to check a vertex is visited or not
	bool* label = new bool[Graph->nVer];
	memset(label, false, Graph->nVer * sizeof(bool));

	// Count vertex reachable from u before remove edge {u,v}
	int countBefore = DFS_Count(u, label);
	
	// Count vertex reachable from u after remove edge {u,v}
	Edge e = removeEdge(u, v);
	memset(label, false, Graph->nVer * sizeof(bool));
	int countAfter = DFS_Count(u, label);

	// Restore {u,v}
	addEdge(u, v, e);

	delete[]label;
	return (countBefore > countAfter);
}

// Recursion to print Euler path/circuit
void Euler::Euler_Rec(int ver) {
	if (ver < 0 || ver >= Graph->nVer || (*Graph)[ver]->get_size() == 0) {
		return;
	}

	Node* p = (*Graph)[ver]->get_head();
	while (p != NULL) {
		if (p->data >= 0) {
			int u = ver, v = p->data;
			if ((*Graph)[u]->get_size() == 1 || !isBrigde(u, v)) {
				printf("%d -> %d \n",u, v);
				removeEdge(u, v);
				Euler_Rec(v);
			}
		}
		p = p->pNext;
	}
}

void Euler::Euler_Print() {
	if (hasCircuit() || hasPath()) {
		if (hasPath()) {
			printf("Duong di Euler:\n");
		}
		else {
			printf("Chu trinh Euler:\n");
		}
		Euler_Rec(startVer);
		printf("\n");
	}
	else {
		printf("Do thi khong co duong di hay chu trinh Euler\n");
	}
}


///////////////////////////////////////////
// Hamilton class

// Constructor
Hamilton::Hamilton(AdjacencyList* g) : Graph(g) {
	// Initialize data
	circuit.resize(0);
	path.resize(0);
	curPath.resize(Graph->nVer, 0);

	// Run algorithm
	(*Graph).printGraph();
	Hamilton_Run();
	if (!circuit.empty()) {
		HamType = 2;
	}
	else if (!path.empty()) {
		HamType = 1;
	}
	else {
		HamType = 0;
	}
}


bool Hamilton::hasCircuit() const {
	return (HamType == 2);
}
bool Hamilton::hasPath() const {
	return (HamType == 1);
}

// Temporary remove edge function
Edge Hamilton::removeEdge(int u, int v) {
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return { -1, -1 ,0 , 0 };;
	}

	Edge e = { u, v , 0, 0 };
	// Remove ver v from adjencyList of u
	Node* p1 = (*Graph)[u]->get_head();
	while (p1 != NULL) {
		if (p1->data == v) {
			p1->data = -1;
			break;
		}
		e.v_pos++;
		p1 = p1->pNext;
	}
	(*Graph)[u]->decreaseSize();

	// Remove ver u from adjencyList of v
	Node* p2 = (*Graph)[v]->get_head();
	while (p2 != NULL) {
		if (p2->data == u) {
			p2->data = -1;
			break;
		}
		e.u_pos++;
		p2 = p2->pNext;
	}
	(*Graph)[v]->decreaseSize();

	return e;
}

// Temporary add edge function
void Hamilton::addEdge(int u, int v, Edge e) {
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return;
	}

	// Add ver v to adjencyList of u
	Node* p1 = (*Graph)[u]->get_head();
	while (p1 != NULL) {
		if (e.v_pos == 0 && p1->data == -1) {
			p1->data = v;
			break;
		}
		e.v_pos--;
		p1 = p1->pNext;
	}
	(*Graph)[u]->decreaseSize();

	// Add ver u to adjencyList of v
	Node* p2 = (*Graph)[v]->get_head();
	while (p2 != NULL) {
		if (e.u_pos == 0 && p2->data == -1) {
			p2->data = u;
			break;
		}
		e.u_pos--;
		p2 = p2->pNext;
	}
	(*Graph)[v]->decreaseSize();
}

// Function to check 2 vertexs are adjacency vertexs
bool Hamilton::isEdge(int u, int v) {
	Node* p = (*Graph)[u]->get_head();
	while (p != NULL) {
		if (p->data == v) {
			return true;
		}
		p = p->pNext;
	}
	return false;
}

// Using DFS to check a Graph is connected or not
// This algorithm is different with check a Euler graph is connected or not
// An Eulerian graph with isolated vertices can still be connected, 
// but a Hamiltonian graph cannot.
bool Hamilton::isConnected() {
	if (Graph->nVer == 1) {
		return true;
	}

	// Initialize an array to check that a vertex is visited or not
	bool* label = new bool[Graph->nVer];
	memset(label, false, Graph->nVer * sizeof(bool));

	// If exists an isolated vertex -> not connected
	for (int i = 0; i < Graph->nVer; i++) {
		if ((*Graph)[i]->get_size() == 0) {
			delete[]label;
			return false;
		}
	}

	// Run DFS
	bool connect = true;
	DFS_visit(Graph, 0, label);
	for (int j = 0; j < Graph->nVer; j++) {
		if (label[j] == false) {
			connect = false;
			break;
		}
	}

	delete[]label;
	return connect;
}

void Hamilton::Hamilton_Run() {
	printf("Nhap dinh bat dau: ");
	int startVer;
	scanf_s("%d", &startVer);

	if (!isConnected()) {
		printf("Do thi khong lien thong nen khong the tim duoc duong di/chu trinh Hamilton\n");
		return;
	}

	// Run Algorithm
	int pos = 1;
	curPath[0] = startVer;
	
	Node* p = (*Graph)[curPath[0]]->get_head();
	while (p != NULL) {
		Hamilton_Rec(pos, p->data);
		p = p->pNext;
	}
}

// Recursion for backtracking 
// Algorithm:
// Initialize an array of nVer elements
// Ex: 
// /////////////////////////////////////////////////////
// // a[0] // a[1] // a[2] // .. // .. // a[nVer - 1] //
// Recursion that satisfy:
// 1. a[i - 1] and a[i] are connected (through isEdge() function)
// 2. for j from 0 to i - 1: a[j] <> a[i]
// 3. if array has nVer elements (by recursion) -> path/ circuit
// - if a[nVer - 1] and a[0] are connected -> Circuit
// - otherwise -> path
void Hamilton::Hamilton_Rec(int pos, int val) {
	curPath[pos] = val;
	if (!isEdge(curPath[pos - 1], curPath[pos])) {
		return;
	}

	Edge e = removeEdge(curPath[pos - 1], curPath[pos]);

	for (int i = 0; i < pos; i++) {
		if (curPath[i] == curPath[pos]) {
			// A vertex duplicate in circuit array
			addEdge(curPath[pos - 1], curPath[pos], e);
			return;
		}
	}

	if (pos == Graph->nVer - 1) {
		// Case forms a circuit
		if (isEdge(curPath[0], curPath[pos])) {
			circuit.push_back(curPath);
		}
		else {
			path.push_back(curPath);
		}
		addEdge(curPath[pos - 1], curPath[pos], e);
		return;
	}

	// Recursion
	Node* p = (*Graph)[curPath[pos]]->get_head();
	while (p != NULL) {
		if (p->data >= 0) {
			Hamilton_Rec(pos + 1, p->data);
		}
		p = p->pNext;
	}
	addEdge(curPath[pos - 1], curPath[pos], e);
}

// Print Hamilton path/circuit
void Hamilton::Ham_print() {
	if (hasCircuit()) {
		printf("Chu trinh Hamilton xuat phat tu dinh %d:\n", curPath[0]);
		// Print all circuit
		for (int i = 0; i < circuit.size(); i++) {
			printf("%d ", circuit[i][0]);
			for (int j = 1; j < Graph->nVer; j++) {
				printf("-> %d ", circuit[i][j]);
			}
			printf("-> %d\n", circuit[i][0]);
		}
		return;
	}
	if (hasPath()) {
		printf("Duong di Hamilton xuat phat tu dinh %d:\n", curPath[0]);
		// Print all circuit
		for (int i = 0; i < circuit.size(); i++) {
			printf("%d ", circuit[i][0]);
			for (int j = 1; j < Graph->nVer; j++) {
				printf("-> %d ", circuit[i][j]);
			}
			printf("\n");
		}
		return;
	}
}

void DFS_visit(AdjacencyList* g, int ver, bool* label) {
	label[ver] = true;

	// DFS for all vertexs in linked list of vertex ver
	Node* p = (*g)[ver]->get_head();
	while (p != NULL) {
		if (label[p->data] == false) {
			DFS_visit(g, p->data, label);
		}
		p = p->pNext;
	}
}