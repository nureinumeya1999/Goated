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


template <typename T>
struct GraphNode {

	String id;
	T* data;
	bool weighted;
	SinglyLinkedList<Neighbor<GraphNode<T>>>* children;
	SinglyLinkedList<Neighbor<GraphNode<T>>>* parents;

	GraphNode(String id, T& data, const bool weighted=false) 
		: id(id), data(&data), weighted(weighted) {

		children = new SinglyLinkedList<Neighbor<GraphNode<T>>>();
		parents = new SinglyLinkedList<Neighbor<GraphNode<T>>>();
	}


	GraphNode(String id, const bool weighted = false) 
		: id(id), data(nullptr), weighted(weighted) {

		children = new SinglyLinkedList<Neighbor<GraphNode<T>>>();
		parents = new SinglyLinkedList<Neighbor<GraphNode<T>>>();

	}


	std::string to_string(bool formatted=true) const {

		std::stringstream ss;
		std::string sep = formatted ? "  " : " ~ ";
		
		ss << "GraphNode[ id: " << id.to_string() << sep << "children: ["; 
		
		Node<Neighbor<GraphNode<T>>>* childPtr = children->head;
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

		Node<Neighbor<GraphNode<T>>>* parentPtr = parents->head;
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
};


typedef std::tuple<std::string, std::string, double> weighted_edge;
template <typename T>
class Graph {

public:

	std::string graphId;
	bool weighted;
	HashTable<GraphNode<T>>* nodes;
	SinglyLinkedList<String>* ids;
	size_t count;

public:

	//constructors 

	Graph(const std::string& title = "", size_t size = NULL, bool weighted = false) {
		graphId = title;
		size_t N = size ? size : 256;
		this->nodes = new HashTable<GraphNode<T>>(N);
		this->weighted = weighted;
		ids = new SinglyLinkedList<String>;
	}

	Graph(std::string (&nodes)[], std::string (&edges)[], const std::string& title="", 
		size_t size=NULL) {
		
		init_members(title, false);
		size_t N = size ? size : 256;
		this->nodes = new HashTable<GraphNode<T>>(N);
		init_nodes(nodes, false);
		init_edges(edges);
	}


	Graph(std::vector<std::string> &nodes, std::vector<std::string>& edges,
		const std::string& title = "", size_t size = NULL) {

		init_members(title, false);
		size_t N = size ? size : nodes.size();
		this->nodes = new HashTable<GraphNode<T>>(N);
		init_nodes(nodes, false);
		init_edges(edges);
	}


	Graph(std::vector<std::string>& nodes, weighted_edge (& edges)[],
		const std::string& title = "", size_t size = NULL) {

		init_members(title, true);
		size_t N = size ? size : nodes.size();
		this->nodes = new HashTable<GraphNode<T>>(N);
		init_nodes(nodes, true);
		init_edges(edges);

	}


	Graph(std::string (& nodes)[], weighted_edge (& edges)[],
		const std::string& title = "", size_t size = NULL) {

		init_members(title, true);
		size_t N = size ? size : 256;
		this->nodes = new HashTable<GraphNode<T>>(N);
		init_nodes(nodes, true);
		init_edges(edges);
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

		GraphNode<T>* parentNode = get_node(parent);
		GraphNode<T>* childNode = get_node(child);

		Neighbor<GraphNode<T>>* parentNeighbor = create_neighbor(childNode, parent_to_child_weight);
		Neighbor<GraphNode<T>>* childNeighbor = create_neighbor(parentNode, parent_to_child_weight);

		if (!parentNode->children->contains_id(*parentNeighbor->id)) {
			parentNode->children->append(*parentNeighbor);
		}
		if (!childNode->parents->contains_id(*childNeighbor->id)) {
			childNode->parents->append(*childNeighbor);
		}
		
	}
	

	insert(const std::string& node, std::string(&children)[], std::string(&parents)[]) {

		validate_weight(false);
		if (!get_node(node)) {
			create_node(node);
		}

		insert_as_child(node, parents);
		insert_as_parent(node, children);
	}


	void insert_as_parent(const std::string& node, std::string(&children)[]) {

		validate_weight(false);
		std::string* ptr = children;
		while (*ptr != "") {
			std::string child = *ptr;
			insert_as_child(node, child);
			ptr += 1;
		}
	}


	void insert_as_parent(const std::string& node, const std::string& childId) {

		validate_weight(false);
		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;
		if (!get_node(childId)) {
			create_node(childId);
		}
		make_edge(nodeId, get_node(childId)->id);
	}


	void insert_as_child(const std::string& node, std::string (&parents)[]) {

		validate_weight(false);
		std::string* ptr = parents;
		while (*ptr != "") {
			std::string parent = *ptr;
			insert_as_child(node, parent);
			ptr += 1;
		}
	}


	void insert_as_child(const std::string& node, const std::string& parentId) {

		validate_weight(false);

		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;
		if (!get_node(parentId)) {
			create_node(parentId);
		}
		make_edge(get_node(parentId)->id, nodeId);
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
		std::string parent = std::get<1>(edge);
		std::string child = std::get<2>(edge);
		double weight = std::get<3>(edge);
		make_edge(parent, child, weight);
	}


	void insert(const std::string& parentId, const std::string& childId, double weight) {
		validate_weight(true);
		make_edge(parentId, childId, weight);
	}


	void remove_edge(const std::string& parentId, const std::string& childId) {
		GraphNode<T>* parent = get_node(parentId), *child = get_node(childId);
		if (parent && child) {
			remove_edge(parent->id, child->id);
		}	
	}
	
	void remove_node(const std::string& nodeId) {
		GraphNode<T>* node = get_node(nodeId);
		remove_node(node->id);}


	void swap_nodes(const std::string& id1, const std::string& id2) {
		swap_nodes(get_node(id1)->id, get_node(id2)->id);}


	std::string to_string(bool formatted = true) const {

		std::stringstream ss;
		ss << "__Graph__{id: " << graphId << "\n";

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
		ss << "count: " << count << "}\n";
		return ss.str();
	}


	typedef bool(*callType)(const std::string&);

	template<size_t N>
	void depth_first_search(const std::string& startId, std::string (&memo)[N],
		callType func = nullptr) {

		SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;

		depth_first_search(startId, memoList, func);

		Node<String>* ptr = memoList->head;
		size_t i = 0;
		while (ptr) {
			memo[i] = ptr->data->to_string();
			ptr = ptr->next;
			i++;
		}
	}


	void depth_first_search(const std::string& startId, std::vector<std::string>& memo,
		callType func = nullptr) {

		SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;

		depth_first_search(startId, memoList, func);

		Node<String>* ptr = memoList->head;
		while (ptr) {
			memo.push_back(ptr->data->to_string());
			ptr = ptr->next;
		}
	}


	template<size_t N>
	void breadth_first_search(std::string (&startIds)[], 
		std::vector<std::vector<std::string>> &memo, callType func=nullptr) {

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
			while (ptr) {
				memo[i].push_back(ptr->data->to_string());
				ptr = ptr->next;
			}
		}
	}


	void breadth_first_search(const std::string& startId, std::vector<std::string>& memo,
		callType func = nullptr) {

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
	}


	template<size_t N>
	void breadth_first_search(const std::string& startId, std::string (&memo)[N],
		callType func = nullptr) {

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
	}


	template<int N>
	void multi_directional_search(std::string(&startIds)[],
		std::vector<std::vector<std::string>>& memo, callType func = nullptr) {

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
	}


	/* If possible, populates the memo with a sorted list of the nodes; where each node supercedes
	all of its parents. */
	void topological_sort(std::string(&memo)[]) {
		SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;
		topological_sort(memoList);
		Node<String>* ptr = memoList->head;
		size_t i = 0;
		while (ptr) {
			memo[i] = ptr->data->to_string();
			ptr = ptr->next;
			i++;
		}
	}


	/* Searches the nodes depth first, picking a node to start at and finding its further parent, 
	then starting the traversal from there. Repeats until all nodes are traversed. */
	void forest_depth_first_search(std::vector<std::vector<std::string>>& memo,
		callType func = nullptr) {

		SinglyLinkedList<SinglyLinkedList<String>>* memoList = \
			new SinglyLinkedList<SinglyLinkedList<String>>;
		forest_depth_first_search(memoList);

		Node<SinglyLinkedList<String>>* ptr = memoList->head;
		while (ptr) {
			Node<String>* ptr2 = ptr->data->head;
			std::vector<std::string>* currPath = new std::vector<std::string>;
			while (ptr2) {
				currPath->push_back(ptr->data->to_string());
				ptr2 = ptr2->next;
			}
			memo.push_back(*currPath);
			ptr = ptr->next;
		}
	}

	
	/* Transposes the graph in-place. */
	void transpose() {
		SinglyLinkedList<String>* keys = nodes->keys();
		Node<String>* ptr = keys->head;
		while (ptr) {
			String id = *ptr->data;
			GraphNode<T>* node = get_node(id);
			SinglyLinkedList<Neighbor<GraphNode<T>>>* temp = node->children;
			node->children = node->parents;
			node->parents = temp;
			ptr = ptr->next;
		}
		std::cout << "Transposed; returned\n" << to_string() << std::endl;
	}
	

	/* Finds the strongly connected components of the graph. A subgraph (U, E_U) ⊂(V, E) = G of graph G is
	strongly connected iff ∀v, w ∈ U, ∃ path (v, . . ., w) contained in (U, E_U). */
	void kosarajus_algorithm(std::vector<std::vector<std::string>>& memo) {

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
	}

	/* Uses concurrent depth-first-searches to find a path from each startId ending at a common node. */
	template<int N>
	void multi_directional_search(std::string(&startIds)[N],
		std::vector<std::vector<std::string>>& memoIntersection,
		callType func = nullptr) {

		
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
	}


protected:

	void validate_weight(bool weight) {

		if (weight && !weighted) {
			throw std::invalid_argument("Cannot create a weighted node in an unweighted graph.");
		}
		if (!weight && weighted) {
			throw std::invalid_argument("Cannot create an unweighted node in a weighted graph.");
		}
	}


	void init_members(const std::string& title = "", const bool weighted = false) {

		this->weighted = weighted;
		count = 0;
		ids = new SinglyLinkedList<String>();
		if (title != "") {
			graphId = title;
		}
		else {
			std::stringstream ss;
			ss << graphIds++;
			graphId = ss.str();
		}
	}


	void init_nodes(std::vector<std::string>& nodes, const bool weighted = false) {

		for (int i = 0; i < nodes.size(); i++) {
			std::string node = nodes[i];
			if (!get_node(node)) {
				create_node(node, weighted);
			}
		}
	}


	void init_nodes(std::string(&nodes)[], const bool weighted = false) {

		std::string* ptr = nodes;
		while (*ptr != "") {
			std::string node = *ptr;
			if (!get_node(node)) {
				create_node(node, weighted);
			}
			ptr++;
		}
	}


	void init_edges(std::string(&edges)[]) {

		validate_weight(false);
		std::string* ptr = edges;
		while (*ptr != "") {
			std::string parent = *ptr;
			std::string child = *(ptr + 1);
			if (!get_node(parent)->children->contains_id(get_node(child)->id)) {
				make_edge(parent, child, -1);
			}
			ptr += 2;
		}
	}


	void init_edges(std::vector<std::string>& edges) {

		validate_weight(false);
		for (int i = 0; i < edges.size(); i += 2) {
			std::string parent = edges[i];
			std::string child = edges[i + 1];
			if (!get_node(parent)->children->contains_id(get_node(child)->id)) {
				make_edge(parent, child, -1);
			}
		}
	}


	void init_edges(weighted_edge(&edges)[]) {

		validate_weight(true);
		weighted_edge* ptr = edges;
		while (std::get<1>(*ptr) != "") {
			std::string parent = std::get<0>(*ptr);
			std::string child = std::get<1>(*ptr);
			double weight = std::get<2>(*ptr);

			if (!get_node(parent)->children->contains_id(get_node(child)->id)) {
				make_edge(parent, child, weight);
			}
			ptr += 1;
		}
	}


	GraphNode<T>* get_node(const std::string& id) const { return nodes->get(id); }

	GraphNode<T>* get_node(String& id) const { return nodes->get(id.to_string()); }


	void create_node(const std::string& id, const bool weighted = false) {
		count++;
		String* nodeId = new String(id);
		GraphNode<T>* newGraphNode = new GraphNode<T>(*nodeId, weighted);
		ids->append(*nodeId);
		this->nodes->put(id, *newGraphNode);
	}


	Neighbor<GraphNode<T>>* create_neighbor(GraphNode<T>* node, double weight = -1) {
		Neighbor<GraphNode<T>>* neighbor = new Neighbor<GraphNode<T>>(node, weight);
		return neighbor;
	}

	void remove_edge(String& parentId, String& childId) {
		GraphNode<T>* child		= get_node(childId);
		GraphNode<T>* parent	= get_node(parentId);

		if (parent) {
			SinglyLinkedList<String>* parentChildren = parent->children;
			parentChildren->remove_id(childId);
		}
		if (child) {
			SinglyLinkedList<String>* childParents = child->parents;
			childParents->remove_id(parentId);
		}
	}


	void remove_node(String& nodeId) {
		GraphNode<T>* node		= get_node(nodeId);
		Node<String>* parentPtr = node->parents->head;

		while (parentPtr) {
			Node<String>* temp		= parentPtr->next;
			String &parentId		= *parentPtr->data->node->id;
			GraphNode<T>* parent	= get_node(parentId);

			remove_edge(parentId, nodeId);
			parentPtr = temp;
		}
		Node<String>* childPtr = node->children->head;
		while (childPtr) {
			Node<String>* temp	= childPtr->next;
			String &childId		= *childPtr->data->node->id;
			GraphNode<T>* child = get_node(childId);

			remove_edge(nodeId, childId);
			childPtr = temp;
		}

		ids->remove_val(nodeId.to_string());
		nodes->remove(nodeId.to_string());
		count--;
	}


	void swap_nodes(String& id1, String& id2) {

		GraphNode<T>* node1 = get_node(id1), * node2 = get_node(id2);
		String tempNode1Id = node1->id;
		T* tempNode1Data = node1->data;

		nodes->put(node1->id.to_string(), *node2);
		nodes->put(node2->id.to_string(), *node1);

		node1->id = node2->id;
		node1->data = node2->data;
		node2->id = tempNode1Id;
		node2->data = tempNode1Data;
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

		Node<Neighbor<GraphNode<T>>>* childPtr = get_node(startId)->children->head;
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

		typedef decltype(Graph<T>::depth_traverse_memo_stopcall)* memoStopCallType;
		memoStopCallType memoStopCallPtr = &Graph<T>::depth_traverse_memo_stopcall;

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
		std::cout << "Depth first search finished. Returned with \n"
			<< "Start Id = " << startId << ": " << memo->to_string(false) << "\n" << std::endl;
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
					GraphNode<T>* nextNode = get_node(*nextInQueue);
					Node<Neighbor<GraphNode<T>>>* childPtr = nextNode->children->head;
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

		BFSmemoStopCallType BFSmemoStopCallPtr = &Graph<T>::breadth_search_memo_stopcall;
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

		std::stringstream ss;
		ss << "\nBreadth first search finished. Returned with {\n";
		for (int i = 0; i < N; i++) {
			ss << "Start Id = " << startIds[i] << ": "
				<< memo[i]->to_string(false) << "\n";
		}
		ss << "}\n";
		std::cout << ss.str() << std::endl;
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
		
		if (*callNum == 0) {
			std::cout << "Performing a mutlidirectional search on graph <"
				<< title << ">" << std::endl;
		}
		std::cout << "call " << (*callNum)++ << ": executing.. " << std::endl;

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
				std::cout << "Path seen: " << newPath->to_string(false) << std::endl;
			}
			else {
				SinglyLinkedList<String>* currPath = memo_paths[index]->get(lastId.to_string());
				SinglyLinkedList<String>* newPath = currPath->copy();
				newPath->append(currId);
				memo_paths[index]->put(currId.to_string(), *newPath);
				std::cout << "Path seen: " << newPath->to_string(false) << std::endl;
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

		MDSmemoStopCallType MDSmemoStopCallPtr = &Graph<T>::multi_directional_memo_stopcall;
		callType funcPtr = &Graph<T>::doNothing;

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

		std::stringstream ss;
		ss << "\nMutlidirectional Search finished. ";
		
		if (!memo_intersection[0]) {
			ss << "No intersections";
			std::cout << ss.str() << std::endl;
			return;
		}

		ss << "Returned with{\n";
		for (int i = 0; i < N; i++) {
			ss << "Start Id = " << startIds[i] << ": "
				<< memo_intersection[i]->to_string(false) << "\n";
		}
		ss << "}\n";
		std::cout << ss.str() << std::endl;
		
	}

	// this function is very dangerous, use with caution. 
	static bool doNothing(const std::string& id) {
		return false;
	}


	bool topological_sort_helper(String& currId, SinglyLinkedList<String>* path,
		SinglyLinkedList<String>* seen, Stack<String>* sorted) {

		seen->append(currId);
		path->append(currId);

		Node<Neighbor<GraphNode<T>>>* childPtr = get_node(currId)->children->head;
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


	// If possible, returns a sorted list of the nodes; where each node supercedes 
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


	int find_furthest_parent(String& id, SinglyLinkedList<String>* path, 
		int currDistance, String* memo) {

		GraphNode<T>* node = get_node(id);
		SinglyLinkedList<Neighbor<GraphNode<T>>>* parents = node->parents;
		if (parents->is_empty() || path->contains_val(id.to_string())) {

			std::cout << "Path seen: " << path->to_string() << " CurrID: " << id.to_string()
				<< std::endl;
			*memo = id;
			return currDistance;
		}

		std::cout << "Path seen: " << path->to_string() << " CurrID: " << id.to_string()
			<< std::endl;

		path->append(id);
		Node<Neighbor<GraphNode<T>>>* ptr = parents->head;
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


	void forest_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo,
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
		std::cout << "Forest depth first search finished. Returned with{\n"
			<< memo->to_string() << "\n}" << std::endl;
	}

	
	// Computes the strongly connected components. 
	void kosarajus_algorithm(SinglyLinkedList<SinglyLinkedList<String>>* memo) {

		std::cout << "Beginning Kosaraju's algorithm...\n" << std::endl;
		Stack<String>* nodeStack = new Stack<String>;
		SinglyLinkedList<SinglyLinkedList<String>>* dfs = \
			new SinglyLinkedList<SinglyLinkedList<String>>;
		forest_depth_first_search(dfs);
		transpose();
		Node<SinglyLinkedList<String>>* currNodes = dfs->head;
		SinglyLinkedList<String>* seen = new SinglyLinkedList<String>;

		while (currNodes) {

			while (!currNodes->data->is_empty()) {
				String currId = *currNodes->data->head->data;
				SinglyLinkedList<String>* connectedComponent = new SinglyLinkedList<String>;
				kosaraju_search(currId.to_string(), connectedComponent, currNodes->data, seen);
				currNodes->data->difference(connectedComponent);
				seen->extend(connectedComponent);
				memo->append(*connectedComponent);
				std::cout << "Kosaraju's algorithm found a connected component. Returned with{\n" 
					<< connectedComponent->to_string() << "\n}" << std::endl;
			}
			currNodes = currNodes->next;
		}
		transpose();
		std::cout << "Kosaraju's algorithm finished. Returned with{\n"
			<< memo->to_string() << "\n}" << std::endl;
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

		typedef decltype(Graph<T>::kosaraju_memo_stopcall)* memoStopCallType;
		memoStopCallType memoStopCallPtr = &Graph<T>::kosaraju_memo_stopcall;

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
		std::cout << "Kosaraju Search finished. Returned with \n"
			<< "Start Id = " << startId << ": " << memo->to_string(false) << "\n" << std::endl;
		return;
	}
};