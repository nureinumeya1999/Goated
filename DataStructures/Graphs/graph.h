#pragma once
#include <string>
#include <sstream>
#include "../nodes.h"
#include "../hash.h"
#include "../queue.h"
#include "../stack.h"
#include "../wrappers.h"
#include <iterator>
#include <stdexcept>
#include <vector>

extern int graphIds = 0x000000;

template<typename T>
struct Neighbor {
	T* node;
	double weight;
	String* id;

	Neighbor(T* node, double weight) {
		this->node = node;
		this->weight = weight;
		this->id = &node->id;
	}
	String get_id() {
		return *id;
	}
};


class GraphNode {

public:

	std::string type = "GraphNode";
	String id;
	bool weighted;
	SinglyLinkedList<Neighbor<GraphNode>>* children;
	SinglyLinkedList<Neighbor<GraphNode>>* parents;

public:

	GraphNode(String id, const bool weighted = false) 
		: id(id), weighted(weighted) {

		if (!weighted) {
			this->type = "GraphNode";
		}
		else {
			this->type = "WeightedGraphNode";
		}
		children = new SinglyLinkedList<Neighbor<GraphNode>>();
		parents = new SinglyLinkedList<Neighbor<GraphNode>>();
	}


	virtual std::string to_string(bool formatted=true) const {

		std::stringstream ss;
		std::string sep = formatted ? "  " : " ~ ";
		
		ss << this->type <<"[ id: " << id.to_string() << sep << "children: ["; 
		
		Node<Neighbor<GraphNode>>* childPtr = children->head;
		while (childPtr) {
			if (weighted) { ss << "("; }
			ss << childPtr->data->node->id.to_string();
			if (weighted) {
				ss << ", " << childPtr->data->weight << ")";
			}
			if (childPtr != children->tail) {
				ss << ", ";
			}
			childPtr = childPtr->next;
		}
		ss << "]" << sep << "parents: [";

		Node<Neighbor<GraphNode>>* parentPtr = parents->head;
		while (parentPtr) { 
			if (weighted) { ss << "("; }
			ss << parentPtr->data->node->id.to_string();
			if (weighted) {
				ss << ", " << parentPtr->data->weight << ")";
			}
			if (parentPtr != parents->tail) {
				ss << ", ";
			}
			parentPtr = parentPtr->next;
		}
		ss << "] ]";

		return ss.str();
	}


protected:

	void set_type(const std::string& type) {
		this->type = type;
	}
};


typedef std::tuple<std::string, std::string, double> weighted_edge;

class Graph {

protected:

	std::string type = "Graph";
	std::string graphId;
	bool weighted;
	SinglyLinkedList<String>* ids;
	size_t count;
	bool hasInitialized = false;
	HashTable<GraphNode>* nodes;
	
	void set_type(const std::string& type) {
		this->type = type;
	}

public:

	//constructors 

	Graph(const std::string& title = "", bool weighted = false) {
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
			this->type = "WeightedGraph";
		}
	}


	void initialize(std::string(&nodes)[], 
		std::string(&edges)[], size_t size=NULL) {

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
	}


	void initialize(std::vector<std::string>& nodes, 
		std::vector<std::string>& edges, size_t size = NULL) {

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
	}


	void initialize(std::vector<std::string>& nodes, 
		weighted_edge(&edges)[], size_t size = NULL) {

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
	}


	void initialize(std::string(&nodes)[], 
		weighted_edge(&edges)[], size_t size = NULL) {

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
	}


	bool exists_node(const std::string& id) const {
		if (get_node(id)) {
			return true;
		}
		return false;
	}


	void make_edge(const std::string& parent, const std::string& child, 
		double parent_to_child_weight=-1) {

		if (parent_to_child_weight == -1) {
			validate_weight(false);
		}
		else {
			validate_weight(true);
		}
		
		if (!exists_node(parent)) { create_node(parent); }
		if (!exists_node(child)) { create_node(child); }

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
	}
	

	void insert(const std::string& node, std::string(&children)[], std::string(&parents)[]) {

		validate_weight(false);
		if (!get_node(node)) {
			create_node(node);
		}

		insert(parents, node);
		insert(node, children);
	}


	void insert(const std::string& node, std::string(&children)[]) {

		validate_weight(false);
		std::string* ptr = children;
		while (*ptr != "") {
			std::string child = *ptr;
			insert(node, child);
			ptr += 1;
		}
	}


	void insert(std::string(&parents)[], const std::string& node) {

		validate_weight(false);
		std::string* ptr = parents;
		while (*ptr != "") {
			std::string parent = *ptr;
			insert(parent, node);
			ptr += 1;
		}
	}


	void insert(const std::string& parentId, const std::string& node) {

		validate_weight(false);

		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;
		if (!get_node(parentId)) {
			create_node(parentId);
		}
		make_edge(parentId, node);
	}


	void insert(const std::string& nodeId) {
		if (!get_node(nodeId)) {
			create_node(nodeId);}
	}


	void insert(weighted_edge(&edges)[]) {

		validate_weight(true);
		weighted_edge* ptr = edges;
		while (std::get<1>(*ptr) == "") {
			insert(*ptr);
			ptr += 1;
		}
	}


	void insert(weighted_edge &edge) {
		validate_weight(true);
		std::string parent = std::get<0>(edge);
		std::string child = std::get<1>(edge);
		double weight = std::get<2>(edge);
		make_edge(parent, child, weight);
	}


	void insert(const std::string& parentId, const std::string& childId, double weight) {
		validate_weight(true);
		make_edge(parentId, childId, weight);
	}


	void remove_edge(const std::string& parentId, const std::string& childId) {
		GraphNode* parent = get_node(parentId), *child = get_node(childId);
		if (parent && child) {
			remove_edge(parent->id, child->id);
		}	
	}
	
	void remove_node(const std::string& nodeId) {
		GraphNode* node = get_node(nodeId);
		remove_node(node->id);}


	void swap_nodes(const std::string& id1, const std::string& id2) {
		swap_nodes(get_node(id1)->id, get_node(id2)->id);}


	virtual std::string to_string(bool formatted = true) const {

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
	}


	typedef bool(*callType)(const std::string&);

	template<size_t N>
	void depth_first_search(const std::string& startId, std::string (&memo)[N],
		callType func = nullptr) {

		std::cout << "\nBeginning depth first search..." << std::endl;
		SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;

		depth_first_search(startId, memoList, func);

		Node<String>* ptr = memoList->head;
		size_t i = 0;
		while (ptr) {
			memo[i] = ptr->data->to_string();
			ptr = ptr->next;
			i++;
		}
		std::cout << "Depth first search on " << this->type << " <" << this->graphId
			<< "> finished. Returned with \n"
			<< "Start Id = " << startId << ": " << memoList->to_string(false) << "\n" << std::endl;
	}


	void depth_first_search(const std::string& startId, std::vector<std::string>& memo,
		callType func = nullptr) {

		std::cout << "\nBeginning depth first search..." << std::endl;
		SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;

		depth_first_search(startId, memoList, func);

		Node<String>* ptr = memoList->head;
		while (ptr) {
			memo.push_back(ptr->data->to_string());
			ptr = ptr->next;
		}
		std::cout << "Depth first search on " << this->type << " <" << this->graphId
			<< "> finished. Returned with \n"
			<< "Start Id = " << startId << ": " << memoList->to_string(false) << "\n" << std::endl;;
	}

	template <size_t N>
	void breadth_first_search(std::vector<std::string> &startIds,
		std::vector<std::vector<std::string>>& memo, callType func = nullptr) {

		std::cout << "\nBeginning breadth first search..." << std::endl;
		std::string ids[N]{};
		SinglyLinkedList<String>* memoList[N]{};
		for (size_t i = 0; i < startIds.size(); i++) {
			ids[i] = startIds[i];
			memoList[i] = new SinglyLinkedList<String>;
		}
		std::string(&idsReff)[N] = ids;
		SinglyLinkedList<String>* (&memoReff)[N] = memoList;
		breadth_first_search(idsReff, memoReff, func);

		for (size_t i = 0; i < N; i++) {
			SinglyLinkedList<String>* path = memoReff[i];
			Node<String>* ptr = path->head;
			std::vector<std::string>* newMemo = new std::vector<std::string>;
			while (ptr) {
				newMemo->push_back(ptr->data->to_string());
				ptr = ptr->next;
			}
			memo.push_back(*newMemo);
		}
		std::stringstream ss;
		ss << "Breadth first search on " << this->type << " <" << this->graphId << 
			"> finished. Returned with {\n";

		for (int i = 0; i < N; i++) {
			ss << "Start Id = " << startIds[i] << ": "
				<< memoReff[i]->to_string(false) << "\n";
		}
		ss << "}\n";
		std::cout << ss.str() << std::endl;
	}


	template<size_t N>
	void breadth_first_search(std::string (&startIds)[], 
		std::vector<std::vector<std::string>> &memo, callType func=nullptr) {

		std::cout << "\nBeginning breadth first search..." << std::endl;
		std::string ids[N]{};
		SinglyLinkedList<String>* memoList[N]{};
		for (size_t i = 0; i < N; i++) {
			ids[i] = *(startIds + i);
			memoList[i] = new SinglyLinkedList<String>;
		}
		std::string(&idsReff)[N] = ids;
		SinglyLinkedList<String>* (&memoReff)[N] = memoList;
		breadth_first_search(idsReff, memoReff, func);

		for (size_t i = 0; i < N; i++) {
			SinglyLinkedList<String>* path = memoReff[i];
			Node<String>* ptr = path->head;
			std::vector<std::string>* newMemo = new std::vector<std::string>;
			while (ptr) {
				newMemo->push_back(ptr->data->to_string());
				ptr = ptr->next;
			}
			memo.push_back(*newMemo);
		}
		std::stringstream ss;
		ss << "Breadth first search on " << this->type << " <" << this->graphId <<
			"> finished. Returned with {\n";

		for (int i = 0; i < N; i++) {
			ss << "Start Id = " << startIds[i] << ": "
				<< memoReff[i]->to_string(false) << "\n";
		}
		ss << "}\n";
		std::cout << ss.str() << std::endl;
	}


	void breadth_first_search(const std::string& startId, std::vector<std::string>& memo,
		callType func = nullptr) {

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
	}


	template<size_t N>
	void breadth_first_search(const std::string& startId, std::string (&memo)[N],
		callType func = nullptr) {

		std::cout << "Beginning breadth first search..." << std::endl;
		std::string ids[1](startId);
		std::string(&idsReff)[1] = ids;

		SinglyLinkedList<String>* memoList[1]{};
		memoList[0] = new SinglyLinkedList<String>;
		SinglyLinkedList<String>* (&memoReff)[1] = memoList;

		breadth_first_search(idsReff, memoReff, func);

		SinglyLinkedList<String>* path = memoReff[0];
		Node<String>* ptr = path->head;
		size_t i = 0;
		while (ptr) {
			memo[i] = ptr->data->to_string();
			i++;
			ptr = ptr->next;
		}
		std::stringstream ss;
		ss << "Breadth first search on " << this->type << " <" << this->graphId <<
			"> finished. Returned with {\n" 
		<< "Start Id = " << startId << ": "<< memoReff[0]->to_string(false) << "\n" << "}\n";
		std::cout << ss.str() << std::endl;
	}


	template<int N>
	void multi_directional_search(std::string(&startIds)[],
		std::vector<std::vector<std::string>>& memo, callType func = nullptr) {

		std::cout << "\nBeginning multidirectional search..." << std::endl;
		std::string ids[N]{};
		SinglyLinkedList<String>* memoList[N]{};
		for (size_t i = 0; i < N; i++) {
			ids[i] = *(startIds + i);
			memoList[i] = new SinglyLinkedList<String>;
		}
		std::string(&idsReff)[N] = ids;
		SinglyLinkedList<String>* (&memoReff)[N] = memoList;
		multi_directional_search(idsReff, memoReff, func);

		for (size_t i = 0; i < N; i++) {
			SinglyLinkedList<String>* path = memoReff[i];
			Node<String>* ptr = path->head;
			while (ptr) {
				memo[i].push_back(ptr->data->to_string());
				ptr = ptr->next;
			}
		}
		std::stringstream ss;
		ss << "Mutlidirectional Search on " << this->type << " <"
			<< this->graphId << "> finished. " << "Returned with{\n";

		if (!memoReff[0]) {
			ss << "No intersections.\n}\n";
			std::cout << ss.str() << std::endl;
			return;
		}

		for (int i = 0; i < N; i++) {
			ss << "Start Id = " << startIds[i] << ": "
				<< memoReff[i]->to_string(false) << "\n";
		}
		ss << "}\n";
		std::cout << ss.str() << std::endl;
	}


	/* Uses concurrent depth-first-searches to find a path from each startId ending at a common node. */
	template<int N>
	void multi_directional_search(std::string(&startIds)[N],
		std::vector<std::vector<std::string>>& memoIntersection,
		callType func = nullptr) {

		std::cout << "\nBeginning multidirectional search..." << std::endl;
		SinglyLinkedList<String>* memoList[N]{};
		multi_directional_search(startIds, memoList);
		for (auto memo : memoList) {
			Node<String>* ptr = memo->head;
			std::vector<std::string>* currPath = new std::vector<std::string>;
			while (ptr) {
				currPath->push_back(ptr->data->to_string());
				ptr = ptr->next;
			}
			memoIntersection.push_back(*currPath);
		}
		std::stringstream ss;
		ss << "\nMutlidirectional Search on " << this->type << " <"
			<< this->graphId << "> finished. " << "Returned with{\n";

		if (!memoList[0]) {
			ss << "No intersections.\n}\n";
			std::cout << ss.str() << std::endl;
			return;
		}

		for (int i = 0; i < N; i++) {
			ss << "Start Id = " << startIds[i] << ": "
				<< memoList[i]->to_string(false) << "\n";
		}
		ss << "}\n";
		std::cout << ss.str() << std::endl;
	}


	void post_order_depth_first_search(const std::string& startId,
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
	}


	/* Searches the nodes depth first, picking a node to start at and finding its further parent,
	then starting the traversal from there. Repeats until all nodes are traversed. */
	void forest_depth_first_search(std::vector<std::vector<std::string>>& memo,
		callType func = nullptr) {

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
	}


	void forest_post_order_depth_first_search(std::vector<std::vector<std::string>>& memo) {

		std::cout << "\nBeginning forested post order depth first search..." << std::endl;
		SinglyLinkedList<SinglyLinkedList<String>>* memoList = \
			new SinglyLinkedList<SinglyLinkedList<String>>;

		forest_post_order_depth_first_search(memoList);

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
	}


	/* If possible, populates the memo with a sorted list of the nodes; where each node supercedes
	all of its parents. */
	void topological_sort(std::string(&memo)[]) {

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
	}

	
	/* Transposes the graph in-place. */
	void transpose() {

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
	}
	

	/* Finds the strongly connected components of the graph. A subgraph (U, E_U) ⊂(V, E) = G of graph G is
	strongly connected iff ∀v, w ∈ U, ∃ path (v, . . ., w) contained in (U, E_U). */
	void kosarajus_algorithm(std::vector<std::vector<std::string>>& memo) {

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
	}

	
protected:

	void validate_weight(bool weight) {

		if (weight && !this->weighted) {
			std::cerr << "Cannot create a weighted node in an unweighted " << this->type << "." << std::endl;
			throw std::invalid_argument("Cannot create a weighted node in an unweighted graph.");
		}
		if (!weight && this->weighted) {
			std::cerr << "Cannot create a weighted node in an unweighted " << this->type << "." <<std::endl;
			throw std::invalid_argument("Cannot create an unweighted node in a weighted graph.");
		}
	}


	void init_graph(size_t size = NULL) {

		size_t N = size ? size : 256;
		this->nodes = new HashTable<GraphNode>(N);
	}


	virtual void init_nodes(std::vector<std::string>& nodes, const bool weighted = false) {

		for (int i = 0; i < nodes.size(); i++) {
			std::string node = nodes[i];
			if (!this->get_node(node)) {
				this->create_node(node, weighted);
			}
		}
	}


	virtual void init_nodes(std::string(&nodes)[], const bool weighted = false) {

		std::string* ptr = nodes;
		while (*ptr != "") {
			std::string node = *ptr;
			if (!this->get_node(node)) {
				this->create_node(node, weighted);
			}
			ptr++;
		}
	}


	virtual void is_valid_edge(const std::string& parent, const std::string& child) {
		return;
	}


	virtual std::string info() const {
		std::stringstream ss;
		ss << "count: " << this->count;
		return ss.str();
	}


	void init_edges(std::string(&edges)[]) {

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
	}


	void init_edges(std::vector<std::string>& edges) {

		this->validate_weight(false);
		for (int i = 0; i < edges.size(); i += 2) {
			std::string parent = edges[i];
			std::string child = edges[i + 1];
			if (!this->get_node(parent)->children->contains_id(this->get_node(child)->id)) {
				this->is_valid_edge(parent, child);
				this->make_edge(parent, child, -1);
			}
		}
	}


	void init_edges(weighted_edge(&edges)[]) {

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
	}


	virtual void validate_graph() { return; }

	virtual GraphNode* get_node(const std::string& id) const { return this->nodes->get(id); }

	virtual GraphNode* get_node(String& id) const { return this->nodes->get(id.to_string()); }


	void virtual create_node(const std::string& id, const bool weighted = false) {
		this->count++;
		String* nodeId = new String(id);
		GraphNode* newGraphNode = new GraphNode(*nodeId, weighted);
		this->ids->append(*nodeId);
		this->nodes->put(id, *newGraphNode);
	}


	virtual Neighbor<GraphNode>* create_neighbor(GraphNode* node, double weight = -1) {
		Neighbor<GraphNode>* neighbor = new Neighbor<GraphNode>(node, weight);
		return neighbor;
	}


	void remove_edge(String& parentId, String& childId) {
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
	}


	void remove_node(String& nodeId) {
		GraphNode* node	= this->get_node(nodeId);
		Node<Neighbor<GraphNode>>* parentPtr = node->parents->head;

		while (parentPtr) {
			Node<Neighbor<GraphNode>>* temp = parentPtr->next;
			String &parentId = parentPtr->data->node->id;
			GraphNode* parent = this->get_node(parentId);

			this->remove_edge(parentId, nodeId);
			parentPtr = temp;
		}
		Node<Neighbor<GraphNode>>* childPtr = node->children->head;
		while (childPtr) {
			Node<Neighbor<GraphNode>>* temp	= childPtr->next;
			String &childId	= childPtr->data->node->id;
			GraphNode* child = this->get_node(childId);

			this->remove_edge(nodeId, childId);
			childPtr = temp;
		}

		this->ids->remove_val(nodeId.to_string());
		this->nodes->remove(nodeId.to_string());
		this->count--;
	}


	void swap_nodes(String& id1, String& id2) {

		GraphNode* node1 = this->get_node(id1), * node2 = this->get_node(id2);
		String tempNode1Id = node1->id;

		this->nodes->put(node1->id.to_string(), *node2);
		this->nodes->put(node2->id.to_string(), *node1);

		node1->id = node2->id;
		node2->id = tempNode1Id;
	}


	template <typename memoStopCall, typename callType, 
		typename memoType, typename rcArgType, typename...kwargs>
	void depth_traverse(
		String&			startId, 
		memoStopCall	memostopcall,
		callType		call, 
		memoType		memo, 
		rcArgType		rcarg, 
		kwargs			...args) {

		bool STOP_FLAG = false;
		STOP_FLAG = memostopcall(startId, call, memo, rcarg, args...);
		if (STOP_FLAG) { return; }

		Node<Neighbor<GraphNode>>* childPtr = get_node(startId)->children->head;
		while (childPtr) {
			depth_traverse<memoStopCall, callType, memoType, rcArgType, kwargs...>
				(childPtr->data->node->id, memostopcall, call, memo, rcarg, args...);
			childPtr = childPtr->next;
		}
		return;
	}


	static bool depth_traverse_memo_stopcall(
		String&						id, 
		callType					func,
		SinglyLinkedList<String>*	memo, 
		int*						rcarg, 
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
	}


	void depth_first_search(
		const std::string& startId,
		SinglyLinkedList<String>* memo,
		callType					func = nullptr) {

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
	}


	template<int N, typename memoStopCallType, typename callType,
		typename memoType, typename...kwargs>
	void breadth_traverse(
		const std::string			(& startIds)[N], 
		memoStopCallType			memostopcall,
		callType					call, 
		memoType					memo, 
		kwargs						...args) {
		
		Queue<String>* toVisitNeighborsArray[N]{};
		SinglyLinkedList<String>* visitedArray[N]{};
		bool STOP_FLAG = false;

		for (int i = 0; i < N; i++) {
			toVisitNeighborsArray[i] = new Queue<String>();
			visitedArray[i] = new SinglyLinkedList<String>();
			STOP_FLAG = memostopcall(get_node(startIds[i])->id, *(new String("")), i, 
				call, toVisitNeighborsArray[i], visitedArray, memo, args...);
			if (STOP_FLAG) { return; }
		}

		STOP_FLAG = true;
		for (int i = 0; i < N; i++) {
			if (!toVisitNeighborsArray[i]->is_empty()) {
				STOP_FLAG = false;
				break;
			}
		}

		while (!STOP_FLAG) {
			for (int i = 0; i < N; i++) {
				String* nextInQueue = toVisitNeighborsArray[i]->dequeue();
				if (nextInQueue) {
					GraphNode* nextNode = get_node(*nextInQueue);
					Node<Neighbor<GraphNode>>* childPtr = nextNode->children->head;
					while (childPtr) {
						STOP_FLAG = memostopcall(childPtr->data->node->id, *nextInQueue, i, call, 
							toVisitNeighborsArray[i], visitedArray, memo, args...);
						if (STOP_FLAG) { return; }
						childPtr = childPtr->next;
					}
				}
			}

			STOP_FLAG = true;
			for (int i = 0; i < N; i++) {
				if (!toVisitNeighborsArray[i]->is_empty()) {
					STOP_FLAG = false;
					break;
				}
			}
		}
	}


	template<int N>
	static bool breadth_search_memo_stopcall(

		// args from breadth traverse scope
		String&						currId, 
		String&						lastId,				// not used but passed to memo_stopcall by breadth_traverse
		int							index, 
		callType					call, 
		Queue<String>*				toVisitNeighbors, 
		SinglyLinkedList<String>*	(& visitedArray)[N], 
		SinglyLinkedList<String>*	(& memo)[N], 

		// variadic args from breadth_first_search scope
		int*						callNum, 
		std::string					title) {


		bool STOP_FLAG = false;
		if (!visitedArray[index]->contains_ref(&currId)) {
			STOP_FLAG = call(currId.to_string());
			if (STOP_FLAG) { return STOP_FLAG;  }
			visitedArray[index]->append(currId);
			memo[index]->append(currId);
			toVisitNeighbors->enqueue(currId);
		}
		return STOP_FLAG;
	}


	void breadth_first_search(const std::string& startId, SinglyLinkedList<String>* memo,
		callType func = nullptr) {

		std::string ids[1](startId);
		std::string (& idsReff)[1] = ids;

		SinglyLinkedList<String>* memoList[1](memo);
		SinglyLinkedList<String>* (&memoReff)[1] = memoList;

		breadth_first_search(idsReff, memoReff, func);
	}


	template<int N>
	void breadth_first_search(const std::string(&startIds)[N], SinglyLinkedList<String>* (&memo)[N],
		callType func=nullptr) {

		typedef bool (*BFSmemoStopCallType)(
			String&, 
			String&, 
			int, 
			callType,
			Queue<String>*, 
			SinglyLinkedList<String>* (&)[N], 
			SinglyLinkedList<String>* (&)[N],
			int*, 
			std::string
		);

		BFSmemoStopCallType BFSmemoStopCallPtr = &Graph::breadth_search_memo_stopcall;
		callType funcPtr = func ? func : doNothing;
		int* ptr = new int(0);

		breadth_traverse<N, BFSmemoStopCallType, callType, SinglyLinkedList<String>*(&)[N],
			int*, std::string>(
				startIds,
				BFSmemoStopCallPtr,
				funcPtr,
				memo,
				ptr,
				graphId
			);

	}


	template<int N>
	static bool multi_directional_memo_stopcall(
		// args from breadth traverse scope
		String&										currId,
		String&										lastId,				
		int											index,
		callType									call,
		Queue<String>*								toVisitNeighbors,
		SinglyLinkedList<String>*					(&visitedArray)[N],				
		HashTable<SinglyLinkedList<String>>*		(&memo_paths)[N],

		// variadic args from multi_directional_search scope
		SinglyLinkedList<String>*					(&memo_intersection)[N],
		int*										callNum,
		std::string									title) {
		
		bool STOP_FLAG = false;

		if (!visitedArray[index]->contains_ref(&currId)) {
			STOP_FLAG = call(currId.to_string());
			if (STOP_FLAG) { return STOP_FLAG; }
			visitedArray[index]->append(currId);
			toVisitNeighbors->enqueue(currId);

			if (lastId.to_string() == "") {
				HashTable<SinglyLinkedList<String>>* newMemo = new HashTable<SinglyLinkedList<String>>();
				memo_paths[index] = newMemo;
				SinglyLinkedList<String>* newPath = new SinglyLinkedList<String>();
				newPath->append(currId);
				memo_paths[index]->put(currId.to_string(), *newPath);
			}
			else {
				SinglyLinkedList<String>* currPath = memo_paths[index]->get(lastId.to_string());
				SinglyLinkedList<String>* newPath = currPath->copy();
				newPath->append(currId);
				memo_paths[index]->put(currId.to_string(), *newPath);
			}
		}

		SinglyLinkedList<String>* ids = memo_paths[0]->keys();
		SinglyLinkedList<String>* intersection = ids;

		bool hasIntersect = false;
		for (int i = 0; i < N - 1; i++) {
			if (memo_paths[i + 1]) {
				intersection = intersection->cap(memo_paths[i + 1]->keys());
				if (!intersection->head) {
					hasIntersect = false;
					break;
				}
				else { if (i == N - 2) { hasIntersect = true; } }
			}
			else {
				break;
			}
		}

		if (hasIntersect) {
			STOP_FLAG = true;
			String commonNode = *intersection->head->data;
			for (int i = 0; i < N; i++) {
				memo_intersection[i] = memo_paths[i]->get(commonNode.to_string());
			}
		}

		return STOP_FLAG;
	}


	template<int N>
	void multi_directional_search(std::string (&startIds)[N], 
		SinglyLinkedList<String>* (&memo_intersection)[N]) {

		typedef bool (*MDSmemoStopCallType)(
			String&,
			String&, 
			int, 
			callType,
			Queue<String>*, 
			SinglyLinkedList<String>* (&)[N], 
			HashTable<SinglyLinkedList<String>>* (&)[N],
			SinglyLinkedList<String>* (&)[N],
			int*, 
			std::string
		);

		MDSmemoStopCallType MDSmemoStopCallPtr = &Graph::multi_directional_memo_stopcall;
		callType funcPtr = &Graph::doNothing;

		int* ptr = new int(0);

		HashTable<SinglyLinkedList<String>>* memo_paths[N]{};

		breadth_traverse<N, MDSmemoStopCallType, callType, 
			HashTable<SinglyLinkedList<String>>* (&)[N],
			SinglyLinkedList<String>* (&)[N], int*, std::string>(
				startIds,
				MDSmemoStopCallPtr,
				funcPtr,
				memo_paths,

				memo_intersection,
				ptr,
				graphId
			);
	}

	// this function is very dangerous, use with caution. 
	static bool doNothing(const std::string& id) {
		return false;
	}


	bool topological_sort_helper(String& currId, SinglyLinkedList<String>* path,
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
	void topological_sort(SinglyLinkedList<String>* memo) {

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


	void post_order_depth_first_search(String& startId, SinglyLinkedList<String>* memo, 
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


	virtual void forest_post_order_depth_first_search(
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
		
	}


	int find_furthest_parent(String& id, SinglyLinkedList<String>* path, 
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
	}


	virtual void forest_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo,
		callType func = nullptr) {

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
	void kosarajus_algorithm(SinglyLinkedList<SinglyLinkedList<String>>* memo) {

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


	static bool kosaraju_memo_stopcall(
		String&						id,
		callType					func,
		SinglyLinkedList<String>*	memo,
		int*						rcarg,
		std::string					title,
		SinglyLinkedList<String>*	currNodes,
		SinglyLinkedList<String>*	seen
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


	void kosaraju_search(
		const std::string& startId,
		SinglyLinkedList<String>* memo,
		SinglyLinkedList<String>* currNodes,
		SinglyLinkedList<String>* seen) {

		typedef decltype(Graph::kosaraju_memo_stopcall)* memoStopCallType;
		memoStopCallType memoStopCallPtr = &Graph::kosaraju_memo_stopcall;

		callType funcPtr =  doNothing;
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


};
