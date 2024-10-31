#include "graph.h"

AdjencyList::AdjencyList(const char* filename) {
	LoadGraph(filename);
}

void AdjencyList::LoadGraph(const char* filename) {
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
		while ((p = strtok_s(NULL, " ", &context)) != NULL) {
			lst[i]->push_back(atoi(p));
		}
		i++;
	}
	fclose(fp);
}

// Copy constructor
AdjencyList::AdjencyList(AdjencyList& org) : nVer(org.nVer){
	lst.resize(nVer);
	for (int i = 0; i < nVer; i++) {
		lst[i] = new List(*org.lst[i]);
	}
}

AdjencyList::~AdjencyList() {
	for (int i = 0; i < nVer; i++) {
		delete lst[i];
	}
}

void AdjencyList::printGraph() {
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

List* AdjencyList::operator[](int index) {
	if (index < 0 || index >= nVer) {
		return NULL;
	}
	return lst[index];
}

// Euler constructor
Euler::Euler(AdjencyList* g) : Graph(g) {
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

// Remove and Add Edge function
Edge Euler::removeEdge(int u, int v){
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return { -1, -1 ,0 , 0};
	}

	Edge e = { u, v , 0, 0 };
	// Remove ver v from adjencyList of u
	Node* p1 = (*Graph)[u]->get_head();
	while (p1 != NULL) {
		e.v_pos++;
		if (p1->data == v) {
			p1->data = -1;
			break;
		}
		p1 = p1->pNext;
	}
	(*Graph)[u]->decreaseSize();

	// Remove ver u from adjencyList of v
	Node* p2 = (*Graph)[v]->get_head();
	while (p2 != NULL) {
		e.u_pos++;
		if (p2->data == u) {
			p2->data = -1;
			break;
		}
		p2 = p2->pNext;
	}
	(*Graph)[v]->decreaseSize();

	return e;
}

void Euler::addEdge(int u, int v, Edge e) {
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return;
	}

	// Add ver v to adjencyList of u
	Node* p1 = (*Graph)[u]->get_head();
	while (p1 != NULL) {
		e.v_pos--;
		if (e.v_pos == 0 && p1->data == -1) {
			p1->data = v;
			break;
		}
		p1 = p1->pNext;
	}
	(*Graph)[u]->increaseSize();

	// Add ver u to adjencyList of v
	Node* p2 = (*Graph)[v]->get_head();
	while (p2 != NULL) {
		e.u_pos--;
		if (e.u_pos == 0 && p2->data == -1) {
			p2->data = u;
			break;
		}
		p2 = p2->pNext;
	}
	(*Graph)[v]->increaseSize();
}

// DFS
bool Euler::isConnected() {
	bool* label = new bool[Graph->nVer];
	memset(label, false, Graph->nVer * sizeof(bool));
	int startVer = -1; 
	for (int i = 0; i < Graph->nVer; i++) {
		if ((*Graph)[i]->get_size() != 0) {
			startVer = i;
			break;
		}
	}
	if (startVer == -1) {
		delete[]label;
		return true;
	}
	bool connect = true;
	DFS_visit(Graph, startVer, label);
	for (int j = 0; j < Graph->nVer; j++) {
		if (label[j] == false && (*Graph)[j]->get_size() != 0) {
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
	Node* p = (*Graph)[ver]->get_head();
	while (p != NULL) {
		if (p->data >= 0 && label[p->data] == false) {
			count += DFS_Count(p->data, label);
		}
		p = p->pNext;
	}
	return count;
}

int Euler::checkEulerGraph(int startVer) {
	if (isConnected() == false) {
		return 0;
	}
	int odd = 0;
	for (int i = 0; i < Graph->nVer; i++) {
		if ((*Graph)[i]->get_size() % 2 == 1) {
			odd++;
		}
	}
	if (odd > 2 || (odd == 2 && (*Graph)[startVer]->get_size() %2 == 0)) {
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
	Edge e = removeEdge(u, v);
	int countAfter = DFS_Count(u, label);

	// Restore {u,v}
	addEdge(u, v, e);
	delete[]label;

	return (countBefore > countAfter);
}

void Euler::Euler_Rec(int ver) {
	if (ver < 0 || ver >= Graph->nVer || (*Graph)[ver]->get_size() == 0) {
		// printf("End at ver %d\n", ver);
		return;
	}
	Node* p = (*Graph)[ver]->get_head();
	while (p != NULL) {
		// printf("DEBUG VER %d at val: %d\n", ver, p->data);
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

Hamilton::Hamilton(AdjencyList* g) : Graph(g) {
	circuit.resize(0);
	path.resize(0);
	curPath.resize(Graph->nVer, 0);

	// Run algorithm
	(*Graph).printGraph();
	run();
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

Hamilton::~Hamilton() {
	
}

bool Hamilton::hasCircuit() const {
	return (HamType == 2);
}
bool Hamilton::hasPath() const {
	return (HamType == 1);
}

Edge Hamilton::removeEdge(int u, int v) {
	if (u < 0 || v < 0 || u >= Graph->nVer || v >= Graph->nVer) {
		return { -1, -1 ,0 , 0 };;
	}

	Edge e = { u, v , -1, -1 };
	// Remove ver v from adjencyList of u
	Node* p1 = (*Graph)[u]->get_head();
	while (p1 != NULL) {
		e.v_pos++;
		if (p1->data == v) {
			p1->data = -1;
			break;
		}
		p1 = p1->pNext;
	}
	(*Graph)[u]->decreaseSize();

	// Remove ver u from adjencyList of v
	Node* p2 = (*Graph)[v]->get_head();
	while (p2 != NULL) {
		e.u_pos++;
		if (p2->data == u) {
			p2->data = -1;
			break;
		}
		p2 = p2->pNext;
	}
	(*Graph)[v]->decreaseSize();

	return e;
}

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

bool Hamilton::isConnected() {
	if (Graph->nVer == 1) {
		return true;
	}

	bool* label = new bool[Graph->nVer];
	memset(label, false, Graph->nVer * sizeof(bool));
	for (int i = 0; i < Graph->nVer; i++) {
		if ((*Graph)[i]->get_size() == 0) {
			delete[]label;
			return false;
		}
	}
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

void Hamilton::run() {
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
		// printf("%d -> %d\n", curPath[0], p->data);
		Hamiton_Rec(pos, p->data);
		p = p->pNext;
	}
}

void Hamilton::Hamiton_Rec(int pos, int val) {
	curPath[pos] = val;
	// printf("Hamilton run at %d\tVal = %d\n", pos, val);
	if (!isEdge(curPath[pos - 1], curPath[pos])) {
		return;
	}

	Edge e = removeEdge(curPath[pos - 1], curPath[pos]);
	// (*Graph).printGraph();

	for (int i = 0; i < pos; i++) {
		if (curPath[i] == curPath[pos]) {
			// A vertex duplicate in circuit array
			// printf("Duplicate with pos %d, val = %d\n", i, curPath[i]);
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
			//printf("%d -> %d\n", curPath[pos], p->data);
			Hamiton_Rec(pos + 1, p->data);
		}
		p = p->pNext;
	}
	addEdge(curPath[pos - 1], curPath[pos], e);
}

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

void DFS_visit(AdjencyList* g, int ver, bool* label) {
	label[ver] = true;
	Node* p = (*g)[ver]->get_head();
	while (p != NULL) {
		if (label[p->data] == false) {
			DFS_visit(g, p->data, label);
		}
		p = p->pNext;
	}
}