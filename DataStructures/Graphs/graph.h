#pragma once
#include <string>
#include <sstream>
#include "../nodes.h"
#include "../hash.h"
#include "../queue.h"
#include "../stack.h"
#include "../wrappers.h"



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

	HashTable<GraphNode<T>>* nodes;
	SinglyLinkedList<String>* ids;
	size_t count;

public:

	template <size_t N, size_t M>
	Graph(const std::string (& nodes)[N], const HashTable<std::string[M]>& nodeDict, 
		size_t size=NULL) {

		count = 0;
		ids = new SinglyLinkedList<String>();
		this->nodes = new HashTable<GraphNode<T>>(size? size : N);

		for (const std::string &node : nodes) {
			if (!get_node(node)) {
				create_node(node);
			}
		}
		std::string* keys = nodeDict.keys();
		size_t keyCount = nodeDict.get_count();

		for (size_t i = 0; i < keyCount; i++) {
			std::string &parent = keys[i];
			std::string (* children)[M] = nodeDict.get(parent);

			if (children) {
				for (const std::string &child : *children) {
					if (child != "") {
						make_edge(get_node(parent)->id, get_node(child)->id);
					}
				}
			}
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

		if (!parentNode->children->contains(childNode)) {
			parentNode->children->append(*childNode);
		}
		if (!childNode->parents->contains(parentNode)) {
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
		ss << "__Graph__{\n";

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
	void depth_traverse(String& startId, memoStopCall memostopcall,
		callType call, memoType memo, rcArgType rcarg, kwargs ...args) {

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

	static bool depth_traverse_memo_stopcall(String& id, callType func,
		SinglyLinkedList<String>* memo, int* rcarg, const std::string& title
		) {
		
		if (*rcarg == 0) {
			std::cout << "Performing a depth first search on graph ["
				<< title << "]" << std::endl;
		}
		std::cout << "call " << (*rcarg)++ << ": executing.. " << std::endl;
		

		bool STOP_FLAG = false;
		if (memo->contains(&id)) {
			STOP_FLAG = true;
		}
		else {
			STOP_FLAG = func(id.to_string());
			memo->append(id);
		}
		return STOP_FLAG;
	}


	typedef decltype(Graph<T>::depth_traverse_memo_stopcall)* memoStopCallType;

	static bool print(const std::string& id) {
		bool STOP_FLAG = false;
		std::cout << id << std::endl;
		return STOP_FLAG;
	}


	void depth_first_search(const std::string& startId, SinglyLinkedList<String>* memo,
		callType func=nullptr) {
	
		memoStopCallType memoStopCallPtr = &Graph<T>::depth_traverse_memo_stopcall;
		callType printPtr = &Graph<T>::print;
		callType funcPtr = func ? func : printPtr;

		int* ptr = new int(0);

		depth_traverse<memoStopCallType, callType, SinglyLinkedList<String>*, 
			int*, const std::string&>(

			get_node(startId)->id,
			memoStopCallPtr,
			funcPtr,
			memo,
			ptr,
			"My Graph"

		);
		return;
	}

	
	template<int N, typename breadthMemoStopCallType, typename callType,
		typename memoType, typename...kwargs>
	void breadth_traverse(std::string (& startIds)[N], breadthMemoStopCallType memostopcall,
		callType call, memoType memo, kwargs ...args) {
		
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
	static bool breadth_search_memo_stopcall(String& currId, String& lastId, int index, 
		callType call, Queue<String>* toVisitNeighbors, 
		SinglyLinkedList<String>* (& visitedArray)[N], 
		SinglyLinkedList<String>* memo, int* callNum, const std::string& title) {

		if (*callNum == 0) {
			std::cout << "Performing a breadth first search on graph ["
				<< title << "]" << std::endl;
		}
		std::cout << "call " << (*callNum)++ << ": executing.. " << std::endl;

		bool STOP_FLAG = false;
		if (!visitedArray[index]->contains(&currId)) {
			STOP_FLAG = call(currId.to_string());
			if (STOP_FLAG) { return STOP_FLAG;  }
			visitedArray[index]->append(currId);
			toVisitNeighbors->enqueue(currId);
		}
		return STOP_FLAG;
	}

	

	template<int N>
	void breadth_first_search(std::string(&startIds)[N], SinglyLinkedList<String>* memo,
		callType func=nullptr) {


		typedef bool (*BFSmemoStopCallType)(String&, String&, int, callType,
			Queue<String>*, SinglyLinkedList<String>* (&)[N], SinglyLinkedList<String>*, 
			int*, const std::string&);

		static BFSmemoStopCallType BFSmemoStopCallPtr = &breadth_search_memo_stopcall;
		callType printPtr = &Graph<T>::print;
		callType funcPtr = func ? func : printPtr;

		int* ptr = new int(0);

		breadth_traverse<N, BFSmemoStopCallType, callType, SinglyLinkedList<String>*,
			int*, const std::string&>(
				startIds,
				BFSmemoStopCallPtr,
				funcPtr,
				memo,
				ptr,
				"My Graph"
			);

		return;
	}

	
	void breadth_first_search(const std::string& startId, SinglyLinkedList<String>* memo,
		callType func = nullptr) {
		std::string arr[1](startId);
		std::string (& startIds)[1] = arr;
		breadth_first_search(startIds, memo, func);
	}


};