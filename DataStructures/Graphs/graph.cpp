﻿#include "graph.h"
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
	this->ids = new SinglyLinkedList<String>;

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


std::string Graph::to_string(bool formatted) const {

	std::stringstream ss;
	ss << "\n__" << this->type << "__{id: " << graphId << "\n";

	if (!formatted) {
		ss << nodes->to_string(formatted);
	}
	else {
		Node<String>* currId = ids->head;
		while (currId) {
			ss << get_node(*currId->data)->to_string(formatted) << "\n";
			currId = currId->next;
		}
	}
	ss << this->info() << "}\n";
	return ss.str();
};


void Graph::depth_first_search(const std::string& startId, std::vector<std::string>& memo,
	callType func) {

	std::cout << "\nBeginning depth first search..." << std::endl;
	SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;

	Graph::depth_first_search(startId, memoList, func);

	Node<String>* ptr = memoList->head;
	while (ptr) {
		memo.push_back(ptr->data->to_string());
		ptr = ptr->next;
	}
	std::cout << "Depth first search on " << this->type << " <" << this->graphId
		<< "> finished. Returned with \n"
		<< "Start Id = " << startId << ": " << memoList->to_string(false) << "\n" << std::endl;;
};


void Graph::breadth_first_search(const std::string& startId, std::vector<std::string>& memo,
	callType func) {

	std::cout << "\nBeginning breadth first search..." << std::endl;
	std::string ids[1](startId);
	std::string(&idsReff)[1] = ids;

	SinglyLinkedList<String>* memoList[1]{};
	memoList[0] = new SinglyLinkedList<String>;
	SinglyLinkedList<String>* (&memoReff)[1] = memoList;

	breadth_first_search(idsReff, memoReff, func);

	SinglyLinkedList<String>* path = memoReff[0];
	Node<String>* ptr = path->head;
	while (ptr) {
		memo.push_back(ptr->data->to_string());
		ptr = ptr->next;
	}
	std::stringstream ss;
	ss << "Breadth first search on <" << this->graphId << "> finished. Returned with {\n"
		<< "Start Id = " << startId << ": " << memoReff[0]->to_string(false) << "\n" << "}\n";
	std::cout << ss.str() << std::endl;
};


void Graph::post_order_depth_first_search(const std::string& startId,
	std::vector<std::string>& memo) {

	std::cout << "\nBeginning post order depth first search..." << std::endl;
	SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;
	SinglyLinkedList<String>* seen = new SinglyLinkedList<String>;
	post_order_depth_first_search(get_node(startId)->id, memoList, seen);

	Node<String>* ptr = memoList->head;
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
	SinglyLinkedList<SinglyLinkedList<String>>* memoList = \
		new SinglyLinkedList<SinglyLinkedList<String>>;
	forest_depth_first_search(memoList);

	Node<SinglyLinkedList<String>>* ptr = memoList->head;
	while (ptr) {
		Node<String>* ptr2 = ptr->data->head;
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
	SinglyLinkedList<SinglyLinkedList<String>>* memoList = \
		new SinglyLinkedList<SinglyLinkedList<String>>;

	Graph::forest_post_order_depth_first_search(memoList);

	Node<SinglyLinkedList<String>>* ptr = memoList->head;
	while (ptr) {
		Node<String>* ptr2 = ptr->data->head;
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
	SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;
	topological_sort(memoList);
	Node<String>* ptr = memoList->head;
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
	SinglyLinkedList<String>* keys = nodes->keys();
	Node<String>* ptr = keys->head;
	while (ptr) {
		String id = *ptr->data;
		GraphNode* node = get_node(id);
		SinglyLinkedList<Neighbor<GraphNode>>* temp = node->children;
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
	SinglyLinkedList<SinglyLinkedList<String>>* memoLists = \
		new SinglyLinkedList<SinglyLinkedList<String>>;
	kosarajus_algorithm(memoLists);
	Node<SinglyLinkedList<String>>* ptr = memoLists->head;
	while (ptr) {
		Node<String>* ptr2 = ptr->data->head;
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
		SinglyLinkedList<Neighbor<GraphNode>>* parentChildren = parent->children;
		parentChildren->remove_id(childId);
	}
	if (child) {
		SinglyLinkedList<Neighbor<GraphNode>>* childParents = child->parents;
		childParents->remove_id(parentId);
	}
};


void Graph::remove_node(String& nodeId) {
	GraphNode* node = this->get_node(nodeId);
	Node<Neighbor<GraphNode>>* parentPtr = node->parents->head;

	while (parentPtr) {
		Node<Neighbor<GraphNode>>* temp = parentPtr->next;
		String& parentId = parentPtr->data->node->id;
		GraphNode* parent = this->get_node(parentId);

		this->remove_edge(parentId, nodeId);
		parentPtr = temp;
	}
	Node<Neighbor<GraphNode>>* childPtr = node->children->head;
	while (childPtr) {
		Node<Neighbor<GraphNode>>* temp = childPtr->next;
		String& childId = childPtr->data->node->id;
		GraphNode* child = this->get_node(childId);

		this->remove_edge(nodeId, childId);
		childPtr = temp;
	}

	this->ids->remove_val(nodeId.to_string());
	this->nodes->remove(nodeId.to_string());
	this->count--;
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
	SinglyLinkedList<String>* memo,
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
	SinglyLinkedList<String>*	memo,
	callType					func) {

	typedef decltype(Graph::depth_traverse_memo_stopcall)* memoStopCallType;
	memoStopCallType memoStopCallPtr = &Graph::depth_traverse_memo_stopcall;

	callType funcPtr = func ? func : doNothing;

	int* ptr = new int(0);

	depth_traverse<memoStopCallType, callType, SinglyLinkedList<String>*,
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


void Graph::breadth_first_search(const std::string& startId, SinglyLinkedList<String>* memo,
	callType func) {

	std::string ids[1](startId);
	std::string(&idsReff)[1] = ids;

	SinglyLinkedList<String>* memoList[1](memo);
	SinglyLinkedList<String>* (&memoReff)[1] = memoList;

	breadth_first_search(idsReff, memoReff, func);
}


bool Graph::topological_sort_helper(String& currId, SinglyLinkedList<String>* path,
	SinglyLinkedList<String>* seen, Stack<String>* sorted) {

	seen->append(currId);
	path->append(currId);

	Node<Neighbor<GraphNode>>* childPtr = get_node(currId)->children->head;
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
void Graph::topological_sort(SinglyLinkedList<String>* memo) {

	Stack<String>* sorted = new Stack<String>();
	SinglyLinkedList<String>* seen = new SinglyLinkedList<String>();
	SinglyLinkedList<String>* path = new SinglyLinkedList<String>();
	SinglyLinkedList<String>* keys = nodes->keys();

	Node<String>* idPtr = keys->head;
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


void Graph::post_order_depth_first_search(String& startId, SinglyLinkedList<String>* memo,
	SinglyLinkedList<String>* seen) {
	if (!seen->contains_val(startId.to_string())) {
		seen->append(startId);
		SinglyLinkedList<Neighbor<GraphNode>>* children = get_node(startId)->children;
		Node<Neighbor<GraphNode>>* ptr = children->head;
		while (ptr) {
			post_order_depth_first_search(ptr->data->node->id, memo, seen);
			ptr = ptr->next;
		}
		memo->append(startId);
	}
}


void Graph::forest_post_order_depth_first_search(
	SinglyLinkedList<SinglyLinkedList<String>>* memo) {

	SinglyLinkedList<String>* ids = nodes->keys();
	SinglyLinkedList<String>* seen = new SinglyLinkedList<String>;
	while (!ids->is_empty()) {
		SinglyLinkedList<String>* postDFSmemo = new SinglyLinkedList<String>;
		SinglyLinkedList<String>* currMemo = new SinglyLinkedList<String>;

		String start = *ids->head->data;
		SinglyLinkedList<String>* path = new SinglyLinkedList<String>;
		String* furthestParentMemo = new String("");
		find_furthest_parent(start, path, 0, furthestParentMemo);
		post_order_depth_first_search(*furthestParentMemo, postDFSmemo, seen);

		memo->append(*postDFSmemo);
		ids->difference(postDFSmemo);
	}

};


int Graph::find_furthest_parent(String& id, SinglyLinkedList<String>* path,
	int currDistance, String* memo) {

	GraphNode* node = get_node(id);
	SinglyLinkedList<Neighbor<GraphNode>>* parents = node->parents;
	if (parents->is_empty() || path->contains_val(id.to_string())) {
		*memo = id;
		return currDistance;
	}
	path->append(id);
	Node<Neighbor<GraphNode>>* ptr = parents->head;
	int maxDistance = currDistance;
	String furthestParent{ "" };

	while (ptr) {
		if (!path->contains_val(ptr->data->node->id.to_string())) {
			SinglyLinkedList<String>* newPath = path->copy();
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


 void Graph::forest_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo,
	callType func) {

	SinglyLinkedList<String>* ids = nodes->keys();
	SinglyLinkedList<String>* DFSmemo = new SinglyLinkedList<String>;
	while (!ids->is_empty()) {
		SinglyLinkedList<String>* currMemo = new SinglyLinkedList<String>;
		Node<String>* lastSeen = DFSmemo->tail;
		String start = *ids->head->data;
		SinglyLinkedList<String>* path = new SinglyLinkedList<String>;
		String* furthestParentMemo = new String("");
		find_furthest_parent(start, path, 0, furthestParentMemo);
		depth_first_search(furthestParentMemo->to_string(), DFSmemo, func);

		Node<String>* ptr = lastSeen ? lastSeen->next : DFSmemo->head;
		while (ptr) {
			currMemo->append(*ptr->data);
			ptr = ptr->next;
		}
		memo->append(*currMemo);
		ids->difference(DFSmemo);
	}
}


// Computes the strongly connected components. 
void Graph::kosarajus_algorithm(SinglyLinkedList<SinglyLinkedList<String>>* memo) {

	Stack<String>* nodeStack = new Stack<String>;
	SinglyLinkedList<SinglyLinkedList<String>>* dfs = \
		new SinglyLinkedList<SinglyLinkedList<String>>;
	forest_post_order_depth_first_search(dfs);
	transpose();
	Node<SinglyLinkedList<String>>* currNodes = dfs->head;
	SinglyLinkedList<String>* seen = new SinglyLinkedList<String>;

	while (currNodes) {

		while (!currNodes->data->is_empty()) {
			String currId = *currNodes->data->tail->data;
			SinglyLinkedList<String>* connectedComponent = new SinglyLinkedList<String>;
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
	SinglyLinkedList<String>* memo,
	int* rcarg,
	std::string					title,
	SinglyLinkedList<String>* currNodes,
	SinglyLinkedList<String>* seen
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
	SinglyLinkedList<String>* memo,
	SinglyLinkedList<String>* currNodes,
	SinglyLinkedList<String>* seen) {

	typedef decltype(Graph::kosaraju_memo_stopcall)* memoStopCallType;
	memoStopCallType memoStopCallPtr = &Graph::kosaraju_memo_stopcall;

	callType funcPtr = doNothing;
	int* ptr = new int(0);

	depth_traverse<memoStopCallType, callType, SinglyLinkedList<String>*,
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