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

extern int graphIds = 0x000000;


struct NeighborWeight {
	String neighbor{""};
	double weight;
	String id{""};

	NeighborWeight(String neighbor, double weight) {
		this->neighbor = neighbor;
		this->weight = weight;
		this->id = neighbor;
	}
};


template <typename T>
struct GraphNode {

	String id;
	T* data;
	SinglyLinkedList<GraphNode<T>>* children;
	SinglyLinkedList<GraphNode<T>>* parents;
	SinglyLinkedList<NeighborWeight>* childrenWeights = nullptr;
	SinglyLinkedList<NeighborWeight>* parentWeights = nullptr;


	GraphNode(String id, T& data) : id(id), data(&data) {
		children = new SinglyLinkedList<GraphNode<T>>();
		parents = new SinglyLinkedList<GraphNode<T>>();
	}

	GraphNode(String id) : id(id), data(nullptr) {
		children = new SinglyLinkedList<GraphNode<T>>();
		parents = new SinglyLinkedList<GraphNode<T>>();
	}


	std::string to_string(bool formatted=true) const {

		std::stringstream ss;
		std::string sep = formatted ? "  " : " ~ ";
		
		ss << "GraphNode[ id: " << id.to_string() << sep << "children: ["; 
		
		Node<GraphNode<T>>* childPtr = children->head;
		while (childPtr) {
			ss << childPtr->data->id.to_string();
			if (childrenWeights) {
				ss << childrenWeights->get_id(childPtr->data->id)->weight;
			}
			if (childPtr != children->tail) {
				ss << ", ";
			}
			childPtr = childPtr->next;
		}
		ss << "]" << sep << "parents: [";

		Node<GraphNode<T>>* parentPtr = parents->head;
		while (parentPtr) {
			ss << parentPtr->data->id.to_string();
			if (parentWeights) {
				ss << parentWeights->get_id(childPtr->data->id)->weight;
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

typedef std::tuple<std::string, std::string, int> weighted_edge;
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

	Graph(std::string (&nodes)[], std::string (&edges)[], const std::string& title="", 
		size_t size=NULL) {
		
		init_members(title, false);
		size_t N = size ? size : 256;
		this->nodes = new HashTable<GraphNode<T>>(N);
		init_nodes(nodes);
		init_edges(edges);
	}


	Graph(std::vector<std::string> &nodes, std::vector<std::string>& edges,
		const std::string& title = "", size_t size = NULL) {

		init_members(title, false);
		size_t N = size ? size : nodes.size();
		this->nodes = new HashTable<GraphNode<T>>(N);
		init_nodes(nodes);
		init_edges(edges);
	}


	Graph(std::vector<std::string>& nodes, weighted_edge (& edges)[],
		const std::string& title = "", size_t size = NULL) {

		init_members(title, true);
		size_t N = size ? size : nodes.size();
		this->nodes = new HashTable<GraphNode<T>>(N);
		init_nodes(nodes);
		init_edges(edges);

	}


	Graph(std::string (& nodes)[], weighted_edge (& edges)[],
		const std::string& title = "", size_t size = NULL) {

		init_members(title, true);
		size_t N = size ? size : 256;
		this->nodes = new HashTable<GraphNode<T>>(N);
		init_nodes(nodes);
		init_edges(edges);
	}


	void validate_weight(bool weight) {

		if (weight && !weighted) {
			throw std::invalid_argument("Cannot create a weighted node in an unweighted graph.");
		}
		if (!weight && weighted) {
			throw std::invalid_argument("Cannot create an unweighted node in a weighted graph.");
		}
	}


	void init_members(const std::string& title = "", bool weighted=false) {
		count = 0;
		ids = new SinglyLinkedList<String>();
		this->weighted = weighted;
		if (title != "") {
			graphId = title;
		}
		else {
			std::stringstream ss;
			ss << graphIds++;
			graphId = ss.str();
		}
	}


	void init_nodes(std::vector<std::string>& nodes) {
		for (int i = 0; i < nodes.size(); i++) {
			std::string node = nodes[i];
			if (!get_node(node)) {
				create_node(node);
			}
		}
	}


	void init_nodes(std::string (& nodes)[]) {
		std::string* ptr = nodes;
		while (*ptr != "") {
			std::string node = *ptr;
			if (!get_node(node)) {
				create_node(node);
			}
			ptr++;
		}
	}


	void init_edges(std::string (&edges)[]) {

		validate_weight(false);
		std::string* ptr = edges;
		while (*ptr != "") {
			std::string parent = *ptr;
			std::string child = *(ptr + 1);
			if (!get_node(parent)->children->contains_ref(get_node(child))) {
				make_edge(parent, child);
			}
			ptr += 2;
		}
	}


	void init_edges(std::vector<std::string>& edges) {

		validate_weight(false);
		for (int i = 0; i < edges.size(); i += 2) {
			std::string parent = edges[i];
			std::string child = edges[i + 1];
			if (!get_node(parent)->children->contains_ref(get_node(child))) {
				make_edge(parent, child);
			}
		}
	}


	void init_edges(weighted_edge(&edges)[]) {

		validate_weight(true);
		weighted_edge* ptr = edges;
		while (std::get<1>(*ptr) != "") {
			std::string parent = std::get<1>(*ptr);
			std::string child = std::get<2>(*ptr);
			double weight = std::get<3>(*ptr);

			if (!get_node(parent)->children->contains_ref(get_node(child))) {
				make_edge(parent, child, weight);
			}
			ptr += 1;
		}
	}


	GraphNode<T>* get_node(const std::string& id) const { return nodes->get(id); }

	GraphNode<T>* get_node(String &id) const { return nodes->get(id.to_string()); }


	void create_node(const std::string& id) {
		count++;
		String* nodeId = new String(id);
		GraphNode<T>* newGraphNode = new GraphNode<T>(*nodeId);
		ids->append(*nodeId);
		this->nodes->put(id, *newGraphNode);
	}


	bool exists_node(const std::string& id) const {
		if (get_node(id)) {
			return true;
		}
		return false;
	}


	void make_edge(String& parent, String& child) {

		validate_weight(false);
		make_edge(parent.value, child.value); }


	void make_edge(const std::string& parent, const std::string& child) {

		if (!exists_node(parent)) { create_node(parent); }
		if (!exists_node(child)) { create_node(child); }

		GraphNode<T>* parentNode	= get_node(parent);
		GraphNode<T>* childNode		= get_node(child);

		if (!parentNode->children->contains_ref(childNode)) {
			parentNode->children->append(*childNode);
		}
		if (!childNode->parents->contains_ref(parentNode)) {
			childNode->parents->append(*parentNode);
		}
	}


	void make_edge(const std::string& parent, const std::string& child, double parent_to_child_weight) {

		validate_weight(true);
		make_edge(parent, child);
		GraphNode<T>* parentNode = get_node(parent);
		GraphNode<T>* childNode = get_node(child);

		NeighborWeight* parent_child_weight = new NeighborWeight{childNode->id, parent_to_child_weight};
		NeighborWeight* child_parent_weight = new NeighborWeight{parentNode->id, parent_to_child_weight};

		parentNode->childrenWeights->remove_id(childNode->id);
		parentNode->childrenWeights->append(*parent_child_weight);
		
		childNode->parentWeights->remove_id(parentNode->id);
		childNode->parentWeights->append(*child_parent_weight);
		
	}
	

	void insert(const std::string& node, std::string(&children)[], std::string(&parents)[]) {

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


	void remove_edge(const std::string& parentId, const std::string& childId) {
		GraphNode<T>* parent = get_node(parentId), *child = get_node(childId);
		if (parent && child) {
			remove_edge(parent->id, child->id);
		}	
	}
	

	void remove_edge(String& parentId, String& childId) {
		GraphNode<T>* child		= get_node(childId);
		GraphNode<T>* parent	= get_node(parentId);

		if (parent) {
			SinglyLinkedList<String>* parentChildren = parent->children;
			parentChildren->remove_val(childId);
			if (weighted) {
				parent->childrenWeights->remove_id(childId);
			}
		}
		if (child) {
			SinglyLinkedList<String>* childParents = child->parents;
			childParents->remove_val(parentId);
			if (weighted) {
				child->parentWeights->remove_id(parentId);
			}
		}
	}


	void remove_node(const std::string& nodeId) {
		GraphNode<T>* node = get_node(nodeId);
		remove_node(node->id);}


	void remove_node(String& nodeId) {
		GraphNode<T>* node		= get_node(nodeId);
		Node<String>* parentPtr = node->parents->head;

		while (parentPtr) {
			Node<String>* temp		= parentPtr->next;
			String &parentId		= *parentPtr->data;
			GraphNode<T>* parent	= get_node(parentId);

			remove_edge(parentId, nodeId);
			parentPtr = temp;
		}
		Node<String>* childPtr = node->children->head;
		while (childPtr) {
			Node<String>* temp	= childPtr->next;
			String &childId		= *childPtr->data;
			GraphNode<T>* child = get_node(childId);

			remove_edge(nodeId, childId);
			childPtr = temp;
		}

		ids->remove_val(nodeId);
		nodes->remove(nodeId.to_string());
		count--;
	}


	void swap_nodes(const std::string& id1, const std::string& id2) {
		swap_nodes(get_node(id1)->id, get_node(id2)->id);}


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

		Node<GraphNode<T>>* childPtr = get_node(startId)->children->head;
		while (childPtr) {
			depth_traverse<memoStopCall, callType, memoType, rcArgType, kwargs...>
				(childPtr->data->id, memostopcall, call, memo, rcarg, args...);
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
		
		if (*rcarg == 0) {
			std::cout << "Performing a depth first search on graph <"
				<< title << ">" << std::endl;
		}
		std::cout << "call " << (*rcarg)++ << ": executing.. " << std::endl;
		
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


	static bool print(const std::string& id) {
		bool STOP_FLAG = false;
		std::cout << id << std::endl;
		return STOP_FLAG;
	}


	void depth_first_search(
		const std::string&			startId, 
		SinglyLinkedList<String>*	memo,
		callType					func=nullptr) {
	
		typedef decltype(Graph<T>::depth_traverse_memo_stopcall)* memoStopCallType;
		memoStopCallType memoStopCallPtr = &Graph<T>::depth_traverse_memo_stopcall;
		callType printPtr = &Graph<T>::print;
		callType funcPtr = func ? func : printPtr;

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
			<< "Start Id = " << startId << ": " << memo->to_string(false) << "\n" <<  std::endl;
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
					Node<GraphNode<T>>* childPtr = nextNode->children->head;
					while (childPtr) {
						STOP_FLAG = memostopcall(childPtr->data->id, *nextInQueue, i, call, 
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


		if (*callNum == 0) {
			std::cout << "Performing a breadth first search on graph <"
				<< title << ">" << std::endl;
		}
		std::cout << "call " << (*callNum)++ << ": executing.. " << std::endl;

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
		callType printPtr = &Graph<T>::print;
		callType funcPtr = func ? func : printPtr;

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

	
	void breadth_first_search(const std::string& startId, SinglyLinkedList<String>* memo,
		callType func = nullptr) {
		std::string arr[1](startId);
		std::string (& startIds)[1] = arr;

		SinglyLinkedList<String>* arrM[1](memo);
		SinglyLinkedList<String>* (&memos)[1] = arrM;

		breadth_first_search(startIds, arrM, func);
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
		SinglyLinkedList<String>* (&memo_intersection)[N],
		callType func=nullptr) {

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
		callType funcPtr = func ? func : &Graph<T>::doNothing;

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

		Node<GraphNode<T>>* childPtr = get_node(currId)->children->head;
		while (childPtr) {
			if (path->contains_val(childPtr->data->id)) { return false; }

			if (!seen->contains_val(childPtr->data->id)) {
				bool success = topological_sort_helper(childPtr->data->id, path->copy(), seen, sorted);
				if (!success) { return false; }
			}
			childPtr = childPtr->next; 
		}

		sorted->push(currId);
		return true;
	}


	void topological_sort(SinglyLinkedList<String>* memo) {

		Stack<String>* sorted = new Stack<String>();
		SinglyLinkedList<String>* seen = new SinglyLinkedList<String>();
		SinglyLinkedList<String>* path = new SinglyLinkedList<String>();
		SinglyLinkedList<String>* keys = nodes->keys();

		Node<String>* idPtr = keys->head;
		while (idPtr) {
			if (!seen->contains_val(*idPtr->data)) {
				
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

};