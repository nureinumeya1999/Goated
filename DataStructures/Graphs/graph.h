#pragma once
#include <string>
#include <sstream>
#include "../nodes.h"
#include "../hash.h"
#include "../queue.h"
#include "../stack.h"
#include "../wrappers.h"

extern int graphIds = 0x000000;

template <typename T>
struct GraphNode {

	String id;
	T* data;
	SinglyLinkedList<GraphNode<T>>* children;
	SinglyLinkedList<GraphNode<T>>* parents;


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
			if (childPtr != children->tail) {
				ss << ", ";
			}
			childPtr = childPtr->next;
		}
		ss << "]" << sep << "parents: [";

		Node<GraphNode<T>>* parentPtr = parents->head;
		while (parentPtr) {
			ss << parentPtr->data->id.to_string();
			if (parentPtr != parents->tail) {
				ss << ", ";
			}
			parentPtr = parentPtr->next;
		}
		ss << "] ]";

		return ss.str();
	}
};


template <typename T>
class Graph {

public:

	std::string graphId;
	HashTable<GraphNode<T>>* nodes;
	SinglyLinkedList<String>* ids;
	size_t count;

public:

	template <size_t N, size_t M>
	Graph(const std::string (& nodes)[N], const HashTable<std::string[M]>& nodeDict, 
		size_t size=0, const std::string& title="") {

		count = 0;
		ids = new SinglyLinkedList<String>();
		this->nodes = new HashTable<GraphNode<T>>(size? size : N);

		if (title != "") {
			graphId = title;
		}
		else {
			std::stringstream ss;
			ss << graphIds++;
			graphId = ss.str();
		}

		for (const std::string &node : nodes) {
			if (!get_node(node)) {
				create_node(node);
			}
		}
		SinglyLinkedList<String>* keys = nodeDict.keys();
		
		Node<String>* ptr = keys->head;

		while (ptr) {
			std::string parent = ptr->data->to_string();
			std::string (* children)[M] = nodeDict.get(parent);

			if (children) {
				for (const std::string &child : *children) {
					if (child != "") {
						make_edge(get_node(parent)->id, get_node(child)->id);
					}
				}
			}
			ptr = ptr->next;
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


	template <size_t M, size_t N>
	void insert(const std::string& node, const std::string(&children)[M], 
		std::string(&parents)[N]) {

		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;
		for (const std::string& parent : parents) {
			if (parent != "") {
				if (!get_node(parent)) {
					create_node(parent);
				}
				make_edge(get_node(parent)->id, nodeId);
			}
		}
		for (const std::string &child : children) {
			if (child != "") {
				if (!get_node(child)) {
					create_node(child);
				}
				make_edge(nodeId, get_node(child)->id);
			}
		}
	}

	template <size_t M>
	void insert(const std::string& node, const std::string(&children)[M]) {

		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;

		for (const std::string& child : children) {
			if (child != "") {
				if (!get_node(child)) {
					create_node(child);
				}
				make_edge(nodeId, get_node(child)->id);
			}
		}
	}

	void insert(const std::string& node, const std::string& parentId) {

		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;
		if (!get_node(parentId)) {
			create_node(parentId);
		}
		make_edge(nodeId, get_node(parentId)->id);
	}


	void insert(std::string nodeId) {
		if (!get_node(nodeId)) {
			create_node(nodeId);}
	}


	void remove_edge(const std::string& parentId, const std::string& childId) {
		GraphNode<T>* parent = get_node(parentId), *child = get_node(childId);
		if (parent && child) {
			remove_edge(parent->id, child->id);}
		}
	
	void remove_edge(String& parentId, String& childId) {
		GraphNode<T>* child		= get_node(childId);
		GraphNode<T>* parent	= get_node(parentId);

		if (parent) {
			SinglyLinkedList<String>* parentChildren = parent->children;
			parentChildren->remove(childId);
		}
		if (child) {
			SinglyLinkedList<String>* childParents = child->parents;
			childParents->remove(parentId);
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

		ids->remove(nodeId);
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
		return;
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
				else {
					if (i == N - 2) {
						hasIntersect = true;
					}
				}
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
		Stack<String> sorted = Stack<String>();
		Stack<String>* sorted_ref = &sorted;
		SinglyLinkedList<String> seen = SinglyLinkedList<String>();
		SinglyLinkedList<String>* seen_ref = &seen;
		SinglyLinkedList<String> path = SinglyLinkedList<String>();
		SinglyLinkedList<String>* path_ref = &path;
		SinglyLinkedList<String>* keys = nodes->keys();
		Node<String>* id = keys->head;
		while (id) {
			if (!seen_ref->contains_val(*id->data)) {
				
				bool success = topological_sort_helper(*id->data, path_ref->copy(), seen_ref, sorted_ref);
				if (!success) { return; }
			}
			id = id->next;
		}
		while (sorted_ref->top) {
			String* temp = sorted_ref->pop();
			memo->append(*temp);
		}
	}

};