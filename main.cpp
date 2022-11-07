#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include "DataStructures/lists.h"
#include "DataStructures/hash.h"
#include "DataStructures/wrappers.h"
#include "DataStructures/stack.h"
#include "DataStructures/queue.h"
#include "DataStructures/Graphs/graph.h"
#include "DataStructures/Graphs/weighted_graph.h"

template <typename T>
struct Vertex {
	T node;
	std::vector<T> edges;

	template<typename...kwargs>
	Vertex(T node, kwargs ...edges) : node(node), edges(edges) {}
};



bool print(const std::string& id);
int main() {
	

	std::string nodes[] = { "A", "B", "C", "D" ,"E", "F", "G", "H", ""};

	std::string edges[] = {
		"A", "A",
		"A", "D",
		"A", "B",
		"A", "H",
		"B", "E",
		"C", "B",
		"C", "A",
		"D", "A",
		"D", "B",
		"D", "E",
		"D", "G",
		"F", "F",
		"F", "D",
		"G", "H",
		"H", "H",
		""
	};


	Graph<Int> graph = Graph<Int>(nodes, edges);
	
	std::cout << graph.to_string() << std::endl;

	graph.swap_nodes("A", "D");
	std::cout << graph.to_string() << std::endl;


	std::string iNeighbors[] = {"J", "K", ""};
	std::string iParents[] = { "L", "A", ""};
	graph.insert("I", iNeighbors, iParents);
	std::cout << graph.to_string() << std::endl;

	SinglyLinkedList<String>* lst = new SinglyLinkedList<String>();

	graph.depth_first_search("F", lst);

	std::string arr[12];
	lst->to_array(arr);
	
	SinglyLinkedList<String>* lst2[8]{};
	for (auto &lst : lst2) {
		lst = new SinglyLinkedList<String>;
	}



	//graph.breadth_first_search(nodes, lst2reff);




	std::string MDSids[4] = { "A", "D", "E",  "B"};
	std::string(&MDSidsRef)[4] = MDSids;

	SinglyLinkedList<String>* lst3[4]{};
	SinglyLinkedList<String>* (&lst3reff)[4] = lst3;
	graph.multi_directional_search(MDSidsRef, lst3reff);




	std::vector<std::vector<std::string>> vec_nodes = {
		{ "A", "D" },
		{ "B", "D", "C"},
		{ "C" },
		{ "D", "C" },
		{ "E" },
		{ "F", "B"}
	};

	std::string arr_nodes[][3] = {
		{ "A", "D" },
		{ "B", "D", "C"},
		{ "C" },
		{ "D", "C" },
		{ "E" },
		{ "F", "B"}
	};

	std::vector<std::pair<std::string, 
		std::vector<std::pair<std::string, int>>>> vec_weighted_nodes = {
		{ "A", {{"D", 2}} },
		{ "B", {{"D", 3}, {"C", 1}} },
		{ "C", {} },
		{ "D", {{"C", 0}} },
		{ "E", {} },
		{ "F", {{"B", 1}} }
	};




	weighted_edge weighted_edges[] = {
		{"A", "D", 2},
		{"B", "D", 3},
		{"B", "C", 1},
		{"D", "C", 0},
		{"F", "B", 1},
		{"", "", NULL}
	};

	std::string projects[] = { "A", "B", "C", "D", "E", "F", ""};
	std::string dependencies[] = {
		"A", "D",
		"B", "C",
		"B", "D",
		"D", "C",
		"F", "A",
		"F", "B",
		""
	};

	SinglyLinkedList<String>* lst4 = new SinglyLinkedList<String>();
	Graph<String> graph3 = Graph<String>(projects, dependencies);
	std::cout << graph3.to_string() << std::endl;
	graph3.topological_sort(lst4);
	std::cout << lst4->to_string() << std::endl;

	return 0;
}


bool print(const std::string& id) {
	std::cout << id << std::endl;
	return false;
}
