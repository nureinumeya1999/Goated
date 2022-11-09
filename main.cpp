
#include <iostream>
#include <string>
#include <vector>
#include "DataStructures/lists.h"
#include "DataStructures/hash.h"
#include "DataStructures/wrappers.h"
#include "DataStructures/stack.h"
#include "DataStructures/queue.h"
#include "DataStructures/Graphs/graph.h"

std::string nodes[] = { "A", "B", "C", "D" ,"E", "F", "G", "H", "" };

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

int main() {
	

	


	Graph<Int> graph = Graph<Int>(nodes, edges);
	
	std::cout << graph.to_string() << std::endl;

	std::vector<std::vector<std::string>>* vec = new std::vector<std::vector<std::string>>;

	graph.forest_depth_first_search(*vec);

	std::vector<std::vector<std::string>>* memo = new std::vector<std::vector<std::string>>(8);
	graph.breadth_first_search<8>(nodes, *memo);

	std::vector<std::vector<std::string>> vec_memo{};
	graph.kosarajus_algorithm(vec_memo);


	std::string MDSids[4] = { "A", "D", "E",  "B"};

	std::vector<std::vector<std::string>>* MDSmemo = new std::vector<std::vector<std::string>>;
	graph.multi_directional_search(MDSids, *MDSmemo);


	weighted_edge weighted_edges[] = {
		{"A", "D", 2},
		{"B", "D", 3},
		{"B", "C", 1},
		{"C", "G", 2},
		{"D", "C", 0},
		{"F", "B", 1},
		{"G", "H", 3},
		{"", "", NULL}
	};

	Graph<Int> weighted_graph = Graph<Int>(nodes, weighted_edges, "weighted graph");

	std::cout << weighted_graph.to_string() << std::endl;

	std::string wlist[8];
	weighted_graph.breadth_first_search("A", wlist);

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

	std::string topMemo[8];
	Graph<String> graph3 = Graph<String>(projects, dependencies);
	std::cout << graph3.to_string() << std::endl;
	graph3.topological_sort(topMemo);

	return 0;
}
