#include "graph.h"
#include "binary_heap.h"
extern int graphIds = 0x000000;

Graph::Graph(const std::string& title, bool weighted) {
	if (title != "") {
		this->graphId = title;
	}
	else {
		std::stringstream ss; ss << graphIds++; this->graphId = ss.str();
	}
	this->nodes = new HashTable<GraphNode>;
	this->weighted = weighted;
	this->ids = new SmartList<String>;

	if (weighted) {
		this->type = "[Weighted]Graph";
	}
};


void Graph::initialize(size_t size) {
	std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
	init_graph(size);
	std::cout << this->to_string() << std::endl;
};


void Graph::initialize(std::string(&nodes)[],
	std::string(&edges)[], size_t size) {

	if (this->hasInitialized) {
		std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
		return;
	}
	std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
	size_t N = size ? size : 256;
	init_graph(N);
	init_nodes(nodes, false);
	init_edges(edges);
	validate_graph();
	this->hasInitialized = true;
	std::cout << this->to_string() << std::endl;
};


void Graph::initialize(std::vector<std::string>& nodes,
	std::vector<std::string>& edges, size_t size) {

	if (this->hasInitialized) {
		std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
		return;
	}
	std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
	size_t N = size ? size : nodes.size();
	init_graph(N);
	init_nodes(nodes, false);
	init_edges(edges);
	validate_graph();
	this->hasInitialized = true;
	std::cout << this->to_string() << std::endl;
};


void Graph::initialize(std::vector<std::string>& nodes,
	weighted_edge(&edges)[], size_t size) {

	if (this->hasInitialized) {
		std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
		return;
	}
	std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
	size_t N = size ? size : nodes.size();
	init_graph(N);
	init_nodes(nodes, true);
	init_edges(edges);
	validate_graph();
	this->hasInitialized = true;
	std::cout << this->to_string() << std::endl;
};


void Graph::initialize(std::string(&nodes)[],
	weighted_edge(&edges)[], size_t size) {

	if (this->hasInitialized) {
		std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
		return;
	}
	std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
	size_t N = size ? size : 256;
	init_graph(N);
	init_nodes(nodes, true);
	init_edges(edges);
	validate_graph();
	this->hasInitialized = true;
	std::cout << this->to_string() << std::endl;
};


bool Graph::exists_node(const std::string& id) const {
	if (get_node(id)) {
		return true;
	}
	return false;
};


void Graph::check_make_edge(const std::string& parent, const std::string& child,
	double parent_to_child_weight) {
	if (parent_to_child_weight == -1) {
		validate_weight(false);
	}
	else {
		validate_weight(true);
		if (parent_to_child_weight < 0) {
			std::cerr << "Weight must be positive." << std::endl;
			return;
		}
	}

	if (!exists_node(parent)) {
		std::cerr << "Edge not created: Parent node does not exist." << std::endl;
		throw std::invalid_argument("Parent node does not exist.");
	}
	if (!exists_node(child)) {
		std::cerr << "Edge not created: Child node does not exist." << std::endl;
		throw std::invalid_argument("Child node does not exist.");
	}
};


void Graph::make_edge(const std::string& parent, const std::string& child,
	double parent_to_child_weight) {

	check_make_edge(parent, child, parent_to_child_weight);

	GraphNode* parentNode = get_node(parent);
	GraphNode* childNode = get_node(child);

	Neighbor<GraphNode>* parentNeighbor = create_neighbor(childNode, parent_to_child_weight);
	Neighbor<GraphNode>* childNeighbor = create_neighbor(parentNode, parent_to_child_weight);

	if (!parentNode->children->contains_id(*parentNeighbor->id)) {
		parentNode->children->append(*parentNeighbor);
	}
	if (!childNode->parents->contains_id(*childNeighbor->id)) {
		childNode->parents->append(*childNeighbor);
	}
};


void Graph::insert(const std::string& node, std::string(&parents)[], std::string(&children)[]) {

	validate_weight(false);
	if (!get_node(node)) {
		create_node(node);
	}

	insert(parents, node);
	insert(node, children);
};


void Graph::insert(const std::string& parentId, std::string(&children)[]) {

	validate_weight(false);
	std::string* ptr = children;
	while (*ptr != "") {
		std::string child = *ptr;
		insert(parentId, child);
		ptr += 1;
	}
};


void Graph::insert(std::string(&parents)[], const std::string& childId) {

	validate_weight(false);
	std::string* ptr = parents;
	while (*ptr != "") {
		std::string parent = *ptr;
		insert(parent, childId);
		ptr += 1;
	}
};


void Graph::insert(const std::string& parentId, const std::string& childId) {

	validate_weight(false);

	if (!get_node(parentId)) {
		std::cerr << "Parent node does not exist." << std::endl;
		return;
	}
	if (!get_node(childId)) {
		create_node(childId);
	}
	make_edge(parentId, childId);
};


void Graph::insert(const std::string& nodeId) {
	if (!get_node(nodeId)) {
		create_node(nodeId);
	}
};


void Graph::insert(weighted_edge(&edges)[]) {

	validate_weight(true);
	weighted_edge* ptr = edges;
	while (std::get<1>(*ptr) == "") {
		insert(*ptr);
		ptr += 1;
	}
};


void Graph::insert(weighted_edge& edge) {
	validate_weight(true);
	std::string parent = std::get<0>(edge);
	std::string child = std::get<1>(edge);
	double weight = std::get<2>(edge);
	insert(parent, child, weight);
};


void Graph::insert(const std::string& parentId, const std::string& childId, double weight) {
	validate_weight(true);
	if (weight < 0) {
		std::cerr << "Weight must be positive" << std::endl;
		return;
	}
	if (!get_node(parentId)) {
		std::cerr << "Parent node does not exist." << std::endl;
		return;
	}
	if (!get_node(childId)) {
		create_node(childId);
	}
	make_edge(parentId, childId, weight);
};


void Graph::remove_edge(const std::string& parentId, const std::string& childId) {
	GraphNode* parent = get_node(parentId), * child = get_node(childId);
	if (parent && child) {
		remove_edge(parent->id, child->id);
	}
};


void Graph::remove_node(const std::string& nodeId) {
	GraphNode* node = get_node(nodeId);
	remove_node(node->id);
};


void Graph::swap_nodes(const std::string& id1, const std::string& id2) {
	swap_nodes(get_node(id1)->id, get_node(id2)->id);
};


std::string Graph::to_string() const {

	std::stringstream ss;
	ss << "\n__" << this->type << "__{id: " << graphId << "\n";

	DNode<String>* currId = ids->head;
	while (currId) {
		ss << get_node(*currId->data)->to_string() << "\n";
		currId = currId->next;
	}
	
	ss << this->info() << "}\n";
	return ss.str();
};


void Graph::depth_first_search(const std::string& startId, std::vector<std::string>& memo,
	callType func) {

	std::cout << "\nBeginning depth first search..." << std::endl;
	SmartList<String>* memoList = new SmartList<String>;

	Graph::depth_first_search(startId, memoList, func);

	DNode<String>* ptr = memoList->head;
	while (ptr) {
		memo.push_back(ptr->data->to_string());
		ptr = ptr->next;
	}
	std::cout << "Depth first search on " << this->type << " <" << this->graphId
		<< "> finished. Returned with \n"
		<< "Start Id = " << startId << ": " << memoList->to_string() << "\n" << std::endl;;
};


void Graph::breadth_first_search(const std::string& startId, std::vector<std::string>& memo,
	callType func) {

	std::cout << "\nBeginning breadth first search..." << std::endl;
	std::string ids[1](startId);
	std::string(&idsReff)[1] = ids;

	SmartList<String>* memoList[1]{};
	memoList[0] = new SmartList<String>;
	SmartList<String>* (&memoReff)[1] = memoList;

	breadth_first_search(idsReff, memoReff, func);

	SmartList<String>* path = memoReff[0];
	DNode<String>* ptr = path->head;
	while (ptr) {
		memo.push_back(ptr->data->to_string());
		ptr = ptr->next;
	}
	std::stringstream ss;
	ss << "Breadth first search on <" << this->graphId << "> finished. Returned with {\n"
		<< "Start Id = " << startId << ": " << memoReff[0]->to_string() << "\n" << "}\n";
	std::cout << ss.str() << std::endl;
};


void Graph::post_order_depth_first_search(const std::string& startId,
	std::vector<std::string>& memo) {

	std::cout << "\nBeginning post order depth first search..." << std::endl;
	SmartList<String>* memoList = new SmartList<String>;
	SmartList<String>* seen = new SmartList<String>;
	post_order_depth_first_search(get_node(startId)->id, memoList, seen);

	DNode<String>* ptr = memoList->head;
	while (ptr) {
		memo.push_back(ptr->data->to_string());
		ptr = ptr->next;
	}

	std::cout << "Post order depth first search on " << this->type << " <"
		<< this->graphId << "> finished. Returned with{\n"
		<< memoList->to_string() << "\n}\n" << std::endl;
	return;
};


/* Searches the nodes depth first, picking a node to start at and finding its further parent,
then starting the traversal from there. Repeats until all nodes are traversed. */
void Graph::forest_depth_first_search(std::vector<std::vector<std::string>>& memo,
	callType func) {

	std::cout << "\nBeginning forested depth first search..." << std::endl;
	SmartList<SmartList<String>>* memoList = \
		new SmartList<SmartList<String>>;
	forest_depth_first_search(memoList);

	DNode<SmartList<String>>* ptr = memoList->head;
	while (ptr) {
		DNode<String>* ptr2 = ptr->data->head;
		std::vector<std::string>* currPath = new std::vector<std::string>;
		while (ptr2) {
			currPath->push_back(ptr2->data->to_string());
			ptr2 = ptr2->next;
		}
		memo.push_back(*currPath);
		ptr = ptr->next;
	}
	std::cout << "Forest depth first search on " << this->type << " <"
		<< this->graphId << "> finished. Returned with{\n"
		<< memoList->to_string() << "\n}\n" << std::endl;
};


void Graph::forest_post_order_depth_first_search(std::vector<std::vector<std::string>>& memo) {

	std::cout << "\nBeginning forested post order depth first search..." << std::endl;
	SmartList<SmartList<String>>* memoList = \
		new SmartList<SmartList<String>>;

	Graph::forest_post_order_depth_first_search(memoList);

	DNode<SmartList<String>>* ptr = memoList->head;
	while (ptr) {
		DNode<String>* ptr2 = ptr->data->head;
		std::vector<std::string>* currPath = new std::vector<std::string>;
		while (ptr2) {
			currPath->push_back(ptr2->data->to_string());
			ptr2 = ptr2->next;
		}
		memo.push_back(*currPath);
		ptr = ptr->next;
	}
	std::string s = memoList->to_string();
	std::cout << "Forested post order depth first search on " << this->type << " <"
		<< this->graphId << "> finished. Returned with{\n"
		<< memoList->to_string() << "\n}\n" << std::endl;
};


/* If possible, populates the memo with a sorted list of the nodes; where each node supercedes
all of its parents. */
void Graph::topological_sort(std::string(&memo)[]) {

	std::cout << "\nBeginning topological sort..." << std::endl;
	SmartList<String>* memoList = new SmartList<String>;
	topological_sort(memoList);
	DNode<String>* ptr = memoList->head;
	size_t i = 0;
	while (ptr) {
		memo[i] = ptr->data->to_string();
		ptr = ptr->next;
		i++;
	}
	std::cout << "Topological sort on " << this->type << " <"
		<< this->graphId << "> finished. Returned with{\n"
		<< memoList->to_string() << "\n}\n" << std::endl;
};


/* Transposes the graph in-place. */
void Graph::transpose() {

	std::cout << "\nTransposing..." << std::endl;
	SmartList<String>* keys = nodes->keys();
	DNode<String>* ptr = keys->head;
	while (ptr) {
		String id = *ptr->data;
		GraphNode* node = get_node(id);
		SmartList<Neighbor<GraphNode>>* temp = node->children;
		node->children = node->parents;
		node->parents = temp;
		ptr = ptr->next;
	}
	std::cout << "Transposed; returned\n" << to_string() << "\n" << std::endl;
};


/* Finds the strongly connected components of the graph. A subgraph (U, E_U) ⊂(V, E) = G of graph G is
strongly connected iff ∀v, w ∈ U, ∃ path (v, . . ., w) contained in (U, E_U). */
void Graph::kosarajus_algorithm(std::vector<std::vector<std::string>>& memo) {

	std::cout << "\nBeginning Kosaraju's algorithm..." << std::endl;
	SmartList<SmartList<String>>* memoLists = \
		new SmartList<SmartList<String>>;
	kosarajus_algorithm(memoLists);
	DNode<SmartList<String>>* ptr = memoLists->head;
	while (ptr) {
		DNode<String>* ptr2 = ptr->data->head;
		std::vector<std::string>* connectedComponent = new std::vector<std::string>;
		while (ptr2) {
			connectedComponent->push_back(ptr2->data->to_string());
			ptr2 = ptr2->next;
		}
		memo.push_back(*connectedComponent);
		ptr = ptr->next;
	}
	std::cout << "Kosaraju's algorithm on " << this->type << " <" << this->graphId
		<< "> finished. Returned with{\n"
		<< memoLists->to_string() << "\n}\n" << std::endl;
};


void Graph::dijsktras_algorithm(const std::string& startId, std::map<std::string, std::string>& previous) {
	std::cout << "\nBeginning Dijsktra's Algorithm sort..." << std::endl;
	HashTable<String> previousHash{this->count};
	this->dijsktras_algorithm(startId, previousHash);
	SmartList<String>* keys = previousHash.keys();
	DNode<String>* curr = keys->head;
	while (curr) {
		String* currPrev = previousHash.get(curr->data->to_string());
		previous.emplace(curr->data->to_string(), currPrev->to_string());
		curr = curr->next;
	}
	std::cout << "Dijsktra's algorithm on " << this->type << " <" << this->graphId
		<< "> finished. Returned with{\n"
		<< "Start Id = " << startId << ": " << previousHash.to_string() << "}\n" << std::endl;
};


void Graph::shortest_path(const std::string& startId, const std::string endId, std::vector<std::string>& shortestPath, double& weight) {
	SmartList<String>* lst = new SmartList<String>;
	this->shortest_path(startId, endId, *lst, weight);
}


void Graph::shortest_path(const std::string& startId, const std::string endId, SmartList<String>& shortestPath, double& weight) {
	HashTable<String> previousPath{};
	this->dijsktras_algorithm(startId, previousPath);
	String* curr = new String(endId);

	while (curr->to_string() != "") {
		shortestPath.append(*(new String(curr->to_string())));
		if (curr->to_string() == startId) { break; }
		curr = previousPath.get(curr->to_string());
	}
	weight = 0;
	DNode<String>* weightCurr = shortestPath.head;
	while (weightCurr->next) {
		weight += get_node(*weightCurr->data)->parents->get_id(*weightCurr->next->data)->weight;
		weightCurr = weightCurr->next;
	}
	shortestPath.reverse();
	std::cout << "Shortest Path from <"<<startId << "> to <" <<endId << "> in " << this->type << " <" << this->graphId
		<< "> finished. Returned with{\nPath: "
		<< shortestPath.to_string() << ", Weight: " << weight << "\n}\n" << std::endl;
}


void Graph::validate_weight(bool weight) {

	if (weight && !this->weighted) {
		std::cerr << "Cannot create a weighted node in an unweighted " << this->type << "." << std::endl;
		throw std::invalid_argument("Cannot create a weighted node in an unweighted graph.");
	}
	if (!weight && this->weighted) {
		std::cerr << "Cannot create a weighted node in an unweighted " << this->type << "." << std::endl;
		throw std::invalid_argument("Cannot create an unweighted node in a weighted graph.");
	}
};


void Graph::init_graph(size_t size) {

	size_t N = size ? size : 256;
	this->nodes = new HashTable<GraphNode>(N);
};


void Graph::init_nodes(std::vector<std::string>& nodes, const bool weighted) {

	for (int i = 0; i < nodes.size(); i++) {
		std::string node = nodes[i];
		if (!this->get_node(node)) {
			this->create_node(node, weighted);
		}
	}
};


void Graph::init_nodes(std::string(&nodes)[], const bool weighted) {

	std::string* ptr = nodes;
	while (*ptr != "") {
		std::string node = *ptr;
		if (!this->get_node(node)) {
			this->create_node(node, weighted);
		}
		ptr++;
	}
};


void Graph::is_valid_edge(const std::string& parent, const std::string& child) {
	return;
};


std::string Graph::info() const {
	std::stringstream ss;
	ss << "count: " << this->count;
	return ss.str();
};


 void Graph::init_edges(std::string(&edges)[]) {

	 this->validate_weight(false);
	 std::string* ptr = edges;
	 while (*ptr != "") {
		 std::string parent = *ptr;
		 std::string child = *(ptr + 1);
		 if (!this->get_node(parent)->children->contains_id(this->get_node(child)->id)) {
			 this->is_valid_edge(parent, child);
			 this->make_edge(parent, child, -1);
		 }
		 ptr += 2;
	 }
 };


void Graph::init_edges(std::vector<std::string>& edges) {

	this->validate_weight(false);
	for (int i = 0; i < edges.size(); i += 2) {
		std::string parent = edges[i];
		std::string child = edges[i + 1];
		if (!this->get_node(parent)->children->contains_id(this->get_node(child)->id)) {
			this->is_valid_edge(parent, child);
			this->make_edge(parent, child, -1);
		}
	}
};


void Graph::init_edges(weighted_edge(&edges)[]) {

	this->validate_weight(true);
	weighted_edge* ptr = edges;
	while (std::get<1>(*ptr) != "") {
		std::string parent = std::get<0>(*ptr);
		std::string child = std::get<1>(*ptr);
		double weight = std::get<2>(*ptr);

		if (!this->get_node(parent)->children->contains_id(this->get_node(child)->id)) {
			this->is_valid_edge(parent, child);
			this->make_edge(parent, child, weight);
		}
		ptr += 1;
	}
};


void Graph::validate_graph() { return; };

GraphNode* Graph::get_node(const std::string& id) const { return this->nodes->get(id); };

GraphNode* Graph::get_node(String& id) const { return this->nodes->get(id.to_string()); };


void Graph::create_node(const std::string& id, const bool weighted) {
	this->count++;
	String* nodeId = new String(id);
	GraphNode* newGraphNode = new GraphNode(*nodeId, weighted);
	this->ids->append(*nodeId);
	this->nodes->put(id, *newGraphNode);
};


Neighbor<GraphNode>* Graph::create_neighbor(GraphNode* node, double weight) {
	Neighbor<GraphNode>* neighbor = new Neighbor<GraphNode>(node, weight);
	return neighbor;
};


void Graph::remove_edge(String& parentId, String& childId) {
	GraphNode* child = this->get_node(childId);
	GraphNode* parent = this->get_node(parentId);

	if (parent) {
		SmartList<Neighbor<GraphNode>>* parentChildren = parent->children;
		parentChildren->remove_id(childId);
	}
	if (child) {
		SmartList<Neighbor<GraphNode>>* childParents = child->parents;
		childParents->remove_id(parentId);
	}
};


void Graph::remove_node(String& nodeId) {
	GraphNode* node = this->get_node(nodeId);
	DNode<Neighbor<GraphNode>>* parentPtr = node->parents->head;

	while (parentPtr) {
		DNode<Neighbor<GraphNode>>* temp = parentPtr->next;
		String& parentId = parentPtr->data->node->id;
		GraphNode* parent = this->get_node(parentId);

		this->remove_edge(parentId, nodeId);
		parentPtr = temp;
	}
	DNode<Neighbor<GraphNode>>* childPtr = node->children->head;
	while (childPtr) {
		DNode<Neighbor<GraphNode>>* temp = childPtr->next;
		String& childId = childPtr->data->node->id;
		GraphNode* child = this->get_node(childId);

		this->remove_edge(nodeId, childId);
		childPtr = temp;
	}

	this->ids->remove_val(nodeId.to_string());
	this->nodes->remove(nodeId.to_string());
	this->count--;
	delete(node);
};


void Graph::swap_nodes(String& id1, String& id2) {

	GraphNode* node1 = this->get_node(id1), * node2 = this->get_node(id2);
	String tempNode1Id = node1->id;

	this->nodes->put(node1->id.to_string(), *node2);
	this->nodes->put(node2->id.to_string(), *node1);

	node1->id = node2->id;
	node2->id = tempNode1Id;
};


bool Graph::depth_traverse_memo_stopcall(
	String& id,
	callType					func,
	SmartList<String>* memo,
	int* rcarg,
	std::string					title
) {

	bool STOP_FLAG = false;
	if (memo->contains_ref(&id)) {
		STOP_FLAG = true;
	}
	else {
		STOP_FLAG = func(id.to_string());
		memo->append(id);
	}
	return STOP_FLAG;
};


void Graph::depth_first_search(
	const std::string&			startId,
	SmartList<String>*	memo,
	callType					func) {

	typedef decltype(Graph::depth_traverse_memo_stopcall)* memoStopCallType;
	memoStopCallType memoStopCallPtr = &Graph::depth_traverse_memo_stopcall;

	callType funcPtr = func ? func : doNothing;

	int* ptr = new int(0);

	depth_traverse<memoStopCallType, callType, SmartList<String>*,
		int*, std::string>(

			get_node(startId)->id,
			memoStopCallPtr,
			funcPtr,
			memo,
			ptr,
			graphId

			);
	return;
};


void Graph::breadth_first_search(const std::string& startId, SmartList<String>* memo,
	callType func) {

	std::string ids[1](startId);
	std::string(&idsReff)[1] = ids;

	SmartList<String>* memoList[1](memo);
	SmartList<String>* (&memoReff)[1] = memoList;

	breadth_first_search(idsReff, memoReff, func);
}


bool Graph::topological_sort_helper(String& currId, SmartList<String>* path,
	SmartList<String>* seen, Stack<String>* sorted) {

	seen->append(currId);
	path->append(currId);

	DNode<Neighbor<GraphNode>>* childPtr = get_node(currId)->children->head;
	while (childPtr) {
		if (path->contains_val(childPtr->data->id->to_string())) { return false; }

		if (!seen->contains_val(childPtr->data->id->to_string())) {
			bool success = topological_sort_helper(*childPtr->data->id, path->copy(), seen, sorted);
			if (!success) { return false; }
		}
		childPtr = childPtr->next;
	}

	sorted->push(currId);
	return true;
}


// If possible, returns a topologically-sorted list of the nodes; where each node supercedes 
// all of its parents. 
void Graph::topological_sort(SmartList<String>* memo) {

	Stack<String>* sorted = new Stack<String>();
	SmartList<String>* seen = new SmartList<String>();
	SmartList<String>* path = new SmartList<String>();
	SmartList<String>* keys = nodes->keys();

	DNode<String>* idPtr = keys->head;
	while (idPtr) {
		if (!seen->contains_val(idPtr->data->to_string())) {

			bool success = topological_sort_helper(*idPtr->data, path->copy(), seen, sorted);
			if (!success) { return; }
		}
		idPtr = idPtr->next;
	}
	while (sorted->top) {
		String* temp = sorted->pop();
		memo->append(*temp);
	}
}


void Graph::post_order_depth_first_search(String& startId, SmartList<String>* memo,
	SmartList<String>* seen) {
	if (!seen->contains_val(startId.to_string())) {
		seen->append(startId);
		SmartList<Neighbor<GraphNode>>* children = get_node(startId)->children;
		DNode<Neighbor<GraphNode>>* ptr = children->head;
		while (ptr) {
			post_order_depth_first_search(ptr->data->node->id, memo, seen);
			ptr = ptr->next;
		}
		memo->append(startId);
	}
}


void Graph::forest_post_order_depth_first_search(
	SmartList<SmartList<String>>* memo) {

	SmartList<String>* ids = nodes->keys();
	SmartList<String>* seen = new SmartList<String>;
	while (!ids->is_empty()) {
		SmartList<String>* postDFSmemo = new SmartList<String>;
		SmartList<String>* currMemo = new SmartList<String>;

		String start = *ids->head->data;
		SmartList<String>* path = new SmartList<String>;
		String* furthestParentMemo = new String("");
		find_furthest_parent(start, path, 0, furthestParentMemo);
		post_order_depth_first_search(*furthestParentMemo, postDFSmemo, seen);

		memo->append(*postDFSmemo);
		ids->difference(postDFSmemo);
	}

};


int Graph::find_furthest_parent(String& id, SmartList<String>* path,
	int currDistance, String* memo) {

	GraphNode* node = get_node(id);
	SmartList<Neighbor<GraphNode>>* parents = node->parents;
	if (parents->is_empty() || path->contains_val(id.to_string())) {
		*memo = id;
		return currDistance;
	}
	path->append(id);
	DNode<Neighbor<GraphNode>>* ptr = parents->head;
	int maxDistance = currDistance;
	String furthestParent{ "" };

	while (ptr) {
		if (!path->contains_val(ptr->data->node->id.to_string())) {
			SmartList<String>* newPath = path->copy();
			String* currNode = new String(ptr->data->node->id.to_string());
			int newDistance = find_furthest_parent(*currNode, newPath, currDistance + 1, memo);
			if (newDistance > maxDistance) {
				maxDistance = newDistance;
				furthestParent = *memo;
			}
			delete(currNode);
			delete(newPath);
		}
		ptr = ptr->next;
	}

	if (furthestParent.to_string() != "") {
		*memo = furthestParent;
	}
	else {
		*memo = id;
	}
	return maxDistance;
};


 void Graph::forest_depth_first_search(SmartList<SmartList<String>>* memo,
	callType func) {

	SmartList<String>* ids = nodes->keys();
	SmartList<String>* DFSmemo = new SmartList<String>;
	while (!ids->is_empty()) {
		SmartList<String>* currMemo = new SmartList<String>;
		DNode<String>* lastSeen = DFSmemo->tail;
		String start = *ids->head->data;
		SmartList<String>* path = new SmartList<String>;
		String* furthestParentMemo = new String("");
		find_furthest_parent(start, path, 0, furthestParentMemo);
		depth_first_search(furthestParentMemo->to_string(), DFSmemo, func);

		DNode<String>* ptr = lastSeen ? lastSeen->next : DFSmemo->head;
		while (ptr) {
			currMemo->append(*ptr->data);
			ptr = ptr->next;
		}
		memo->append(*currMemo);
		ids->difference(DFSmemo);
	}
}


// Computes the strongly connected components. 
void Graph::kosarajus_algorithm(SmartList<SmartList<String>>* memo) {

	Stack<String>* nodeStack = new Stack<String>;
	SmartList<SmartList<String>>* dfs = \
		new SmartList<SmartList<String>>;
	forest_post_order_depth_first_search(dfs);
	transpose();
	DNode<SmartList<String>>* currNodes = dfs->head;
	SmartList<String>* seen = new SmartList<String>;

	while (currNodes) {

		while (!currNodes->data->is_empty()) {
			String currId = *currNodes->data->tail->data;
			SmartList<String>* connectedComponent = new SmartList<String>;
			kosaraju_search(currId.to_string(), connectedComponent, currNodes->data, seen);
			currNodes->data->difference(connectedComponent);
			seen->extend(connectedComponent);
			memo->append(*connectedComponent);
		}
		currNodes = currNodes->next;
	}
	transpose();
}


bool Graph::kosaraju_memo_stopcall(
	String& id,
	callType					func,
	SmartList<String>* memo,
	int* rcarg,
	std::string					title,
	SmartList<String>* currNodes,
	SmartList<String>* seen
) {

	bool STOP_FLAG = false;

	if (memo->contains_ref(&id) || seen->contains_val(id.to_string()) \
		|| !currNodes->contains_val(id.to_string())) {
		STOP_FLAG = true;
	}
	else {
		memo->append(id);
	}
	return STOP_FLAG;
}


void Graph::kosaraju_search(
	const std::string& startId,
	SmartList<String>* memo,
	SmartList<String>* currNodes,
	SmartList<String>* seen) {

	typedef decltype(Graph::kosaraju_memo_stopcall)* memoStopCallType;
	memoStopCallType memoStopCallPtr = &Graph::kosaraju_memo_stopcall;

	callType funcPtr = doNothing;
	int* ptr = new int(0);

	depth_traverse<memoStopCallType, callType, SmartList<String>*,
		int*, std::string>(

			get_node(startId)->id,
			memoStopCallPtr,
			funcPtr,
			memo,
			ptr,
			graphId,

			currNodes,
			seen

			);
	return;
}


void Graph::dijsktras_algorithm(const std::string& startId, 
	HashTable<String>& previous) {

	HashTable<double> pathToNodeWeight{};
	BinaryHeap remaining{};
	remaining.initialize();

	remaining.insert(startId, 0);
	pathToNodeWeight.put(startId, *(new double(0)));
	previous.put(startId, *(new String(startId)));

	SmartList<String>* keys = this->nodes->keys();
	DNode<String>* ptr = keys->head;
	while (ptr) {
		if (ptr->data->to_string() != startId) {
			remaining.insert(ptr->data->to_string(), INFINITY);
			pathToNodeWeight.put(ptr->data->to_string(), *(new double(INFINITY)));
			previous.put(ptr->data->to_string(), *(new String("")));
		}
		ptr = ptr->next;
	}
	while (remaining.count > 0) {
		std::pair<String, Double>* curr = remaining.pop();
		String* currId = new String(std::get<0>(*curr).to_string());
		Double currWeight = std::get<1>(*curr);

		DNode<Neighbor<GraphNode>>* child = get_node(*currId)->children->head;
		while (child) {
			String* childId = &child->data->node->id;
			double currToChildWeight = child->data->weight;

			double* routedWeight = new double(currWeight.value + currToChildWeight);
			double directWeight = *pathToNodeWeight.get(childId->to_string());

			if (*routedWeight < directWeight) {
				pathToNodeWeight.put(childId->to_string(), *routedWeight);
				remaining.update_node(childId->to_string(), *routedWeight);
				previous.put(childId->to_string(), *currId);
			}
			child = child->next;
		}
	}
}