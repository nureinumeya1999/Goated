#include <iostream>
#include <string>
#include <vector>
#include "DataStructures/lists.h"
#include "DataStructures/hash.h"
#include "DataStructures/wrappers.h"
#include "DataStructures/stack.h"
#include "DataStructures/queue.h"
#include "DataStructures/Graphs/graph.h"
#include "DataStructures/Graphs/tree.h"
#include "DataStructures/Graphs/binary_tree.h"
#include "types.h"
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
	

	


	Graph graph = Graph();
	graph.initialize(nodes, edges);
	std::vector<std::string>* depthMemo = new std::vector<std::string>,
		*postMemo = new std::vector<std::string>;
	std::vector<std::vector<std::string>>* forestPostMemo =\
		new std::vector<std::vector<std::string>>;

	graph.depth_first_search("A", *depthMemo);
	graph.post_order_depth_first_search("A", *postMemo);
	graph.forest_post_order_depth_first_search(*forestPostMemo);

	std::vector<std::vector<std::string>>* vec = new std::vector<std::vector<std::string>>;
	graph.forest_depth_first_search(*vec);


	std::vector<std::vector<std::string>>* memo = new std::vector<std::vector<std::string>>;
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

	Graph weighted_graph = Graph("weighted graph", true);
	weighted_graph.initialize(nodes, weighted_edges);

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
	Graph graph3 = Graph("Project Order");
	graph3.initialize(projects, dependencies);
	graph3.topological_sort(topMemo);


	std::string treeEdges[] = {
		"B", "E",
		"E", "F",
		"F", "G",
		"D", "H",
		"A", "B",
		"A", "C",
		"A", "D",
		"", ""
	};
	Tree tree = Tree("My tree", false);
	tree.initialize(nodes, treeEdges);
	std::vector<std::vector<std::string>>* treeMemo = new std::vector<std::vector<std::string>>;
	
	
	std::vector<std::vector<std::string>>* treeMemo1 = new std::vector<std::vector<std::string>>;
	std::vector<std::vector<std::string>>* treeMemo2 = new std::vector<std::vector<std::string>>;
	

	tree.forest_depth_first_search(*treeMemo);
	tree.forest_post_order_depth_first_search(*treeMemo1);
	
	std::string treeArray[8];
	tree.topological_sort(treeArray);


	std::string btreeEdges[] = {
		"A", "B",
		"A", "C",
		"B", "D", 
		"B", "E", 
		"C", "F", 
		"C", "G",
		"G", "H",
		"", ""
	};

	BinaryTree binaryTree{ "My binary tree" };
	binaryTree.initialize(nodes, btreeEdges);
	std::vector<std::string> preOrderT, inOrderT, postOrderT;
	binaryTree.pre_order_traversal(preOrderT);
	binaryTree.in_order_traversal(inOrderT);
	binaryTree.post_order_traversal(postOrderT);

	std::vector<std::vector<std::string>> btreeForest;
	std::vector<std::vector<std::string>> bTreePostForest;
	binaryTree.forest_depth_first_search(btreeForest);
	binaryTree.forest_post_order_depth_first_search(bTreePostForest);

	weighted_edge weighted_tree_edges[] = {
		{"A", "B", 2},
		{"B", "D", 3},
		{"B", "C", 1},
		{"C", "G", 2},
		{"C", "E", 1},
		{"D", "F", 0},
		{"G", "H", 3},
		{"", "", NULL}
	};

	BinaryTree weightedBtree{"weighted b tree", true};
	weightedBtree.initialize(nodes, weighted_tree_edges);

	vec_vec_string wbtreevec;
	weightedBtree.breadth_first_search<8>(nodes, wbtreevec);
	return 0;
}
