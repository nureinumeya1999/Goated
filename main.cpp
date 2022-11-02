#include <iostream>
#include <string>
#include <vector>
#include "DataStructures/lists.h"
#include "DataStructures/hash.h"
#include "DataStructures/wrappers.h"
#include "DataStructures/stack.h"
#include "DataStructures/queue.h"
#include "DataStructures/Graphs/graph.h"

int main() {
	

	std::string nodes[4] = {"A", "B", "C", "D"};


	std::string ANeighbors[4] = {"B", "C"};
	std::string BNeighbors[4] = {"C", "A"};
	std::string CNeighbors[4] = {"D"};
	std::string DNeighbors[4] = {"B"};
	HashTable<std::string[4]> nodeDict{4};
	nodeDict.put("A", ANeighbors);
	nodeDict.put("B", BNeighbors);
	//nodeDict.put("C", CNeighbors);
	nodeDict.put("D", DNeighbors);



	Graph<Int> graph = Graph<Int>(nodes, nodeDict);
	
	std::cout << graph.to_string() << std::endl;

	std::string newId = "E";
	std::string newNeighbors[3] = { "F", "A", "D" };
	std::string parentId = "C";
	graph.insert(newId, newNeighbors, parentId);

	std::cout << graph.to_string(true) << std::endl;

	graph.remove_node("E");
	std::cout << graph.to_string(true) << std::endl;


	graph.remove_edge("A", "B");
	graph.remove_edge("B", "A");
	std::cout << graph.to_string(true) << std::endl;

	graph.insert("G");
	std::cout << graph.to_string(true) << std::endl;

	graph.make_edge("A", "G");
	std::cout << graph.to_string(true) << std::endl;









	return 0;

}


