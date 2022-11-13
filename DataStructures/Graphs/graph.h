#pragma once
#include "graph_node.h"
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

	Graph(const std::string& title = "", bool weighted = false);

	void initialize(size_t size = NULL);


	void initialize(std::string(&nodes)[], std::string(&edges)[], size_t size = NULL);


	void initialize(std::vector<std::string>& nodes, std::vector<std::string>& edges, size_t size = NULL);


	void initialize(std::vector<std::string>& nodes, weighted_edge(&edges)[], size_t size = NULL);


	void initialize(std::string(&nodes)[], weighted_edge(&edges)[], size_t size = NULL);


	bool exists_node(const std::string& id) const;


	void check_make_edge(const std::string& parent, const std::string& child, double parent_to_child_weight = -1);


	void make_edge(const std::string& parent, const std::string& child, double parent_to_child_weight = -1);
	

	void insert(const std::string& node, std::string(&parents)[], std::string(&children)[]);


	void insert(const std::string& parentId, std::string(&children)[]);


	void insert(std::string(&parents)[], const std::string& childId);


	virtual void insert(const std::string& parentId, const std::string& childId);


	void insert(const std::string& nodeId);


	void insert(weighted_edge(&edges)[]);


	void insert(weighted_edge& edge);


	virtual void insert(const std::string& parentId, const std::string& childId, double weight);


	virtual void remove_edge(const std::string& parentId, const std::string& childId);
	

	virtual void remove_node(const std::string& nodeId);


	void swap_nodes(const std::string& id1, const std::string& id2);


	virtual std::string to_string(bool formatted = true) const;


	typedef bool(*callType)(const std::string&);


	template<size_t N>
	void depth_first_search(const std::string& startId, std::string(&memo)[N], callType func = nullptr) {

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


	void depth_first_search(const std::string& startId, std::vector<std::string>& memo, callType func = nullptr);

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
	void breadth_first_search(std::string (&startIds)[], std::vector<std::vector<std::string>> &memo, callType func=nullptr) {

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


	void breadth_first_search(const std::string& startId, std::vector<std::string>& memo, callType func = nullptr);


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
	void multi_directional_search(std::string(&startIds)[], std::vector<std::vector<std::string>>& memo, callType func = nullptr) {

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
	void multi_directional_search(std::string(&startIds)[N], std::vector<std::vector<std::string>>& memoIntersection, callType func = nullptr) {

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


	void post_order_depth_first_search(const std::string& startId, std::vector<std::string>& memo);


	/* Searches the nodes depth first, picking a node to start at and finding its further parent,
	then starting the traversal from there. Repeats until all nodes are traversed. */
	void forest_depth_first_search(std::vector<std::vector<std::string>>& memo, callType func = nullptr);


	void forest_post_order_depth_first_search(std::vector<std::vector<std::string>>& memo);


	/* If possible, populates the memo with a sorted list of the nodes; where each node supercedes
	all of its parents. */
	void topological_sort(std::string(&memo)[]);

	
	/* Transposes the graph in-place. */
	void transpose();
	

	/* Finds the strongly connected components of the graph. A subgraph (U, E_U) ⊂(V, E) = G of graph G is
	strongly connected iff ∀v, w ∈ U, ∃ path (v, . . ., w) contained in (U, E_U). */
	void kosarajus_algorithm(std::vector<std::vector<std::string>>& memo);

	
protected:

	void validate_weight(bool weight);


	virtual void init_graph(size_t size = NULL);


	virtual void init_nodes(std::vector<std::string>& nodes, const bool weighted = false);


	virtual void init_nodes(std::string(&nodes)[], const bool weighted = false);


	virtual void is_valid_edge(const std::string& parent, const std::string& child);


	virtual std::string info() const;


	void init_edges(std::string(&edges)[]);


	void init_edges(std::vector<std::string>& edges);


	void init_edges(weighted_edge(&edges)[]);


	virtual void validate_graph();


	virtual GraphNode* get_node(const std::string& id) const;


	virtual GraphNode* get_node(String& id) const;


	virtual void create_node(const std::string& id, const bool weighted = false);


	virtual Neighbor<GraphNode>* create_neighbor(GraphNode* node, double weight = -1);


	void remove_edge(String& parentId, String& childId);


	void remove_node(String& nodeId);


	void swap_nodes(String& id1, String& id2);


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
	);


	void depth_first_search(const std::string& startId, SinglyLinkedList<String>* memo, callType func = nullptr);


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


	void breadth_first_search(const std::string& startId, SinglyLinkedList<String>* memo, callType func = nullptr);


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
	void multi_directional_search(std::string (&startIds)[N], SinglyLinkedList<String>* (&memo_intersection)[N]) {

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
	static bool doNothing(const std::string& id) { return false;}


	bool topological_sort_helper(String& currId, SinglyLinkedList<String>* path, SinglyLinkedList<String>* seen, Stack<String>* sorted);


	// If possible, returns a topologically-sorted list of the nodes; where each node supercedes 
	// all of its parents. 
	void topological_sort(SinglyLinkedList<String>* memo);


	void post_order_depth_first_search(String& startId, SinglyLinkedList<String>* memo, SinglyLinkedList<String>* seen);


	virtual void forest_post_order_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo);


	int find_furthest_parent(String& id, SinglyLinkedList<String>* path, int currDistance, String* memo);


	virtual void forest_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo, callType func = nullptr);

	
	// Computes the strongly connected components. 
	void kosarajus_algorithm(SinglyLinkedList<SinglyLinkedList<String>>* memo);


	static bool kosaraju_memo_stopcall(
		String&						id,
		callType					func,
		SinglyLinkedList<String>*	memo,
		int*						rcarg,
		std::string					title,
		SinglyLinkedList<String>*	currNodes,
		SinglyLinkedList<String>*	seen
	);


	void kosaraju_search(
		const std::string&			startId,
		SinglyLinkedList<String>*	memo,
		SinglyLinkedList<String>*	currNodes,
		SinglyLinkedList<String>*	seen); 

};
