#include <iostream>
#include <string>
#include <vector>
#include "DataStructures/lists.h"
#include "DataStructures/hash.h"
#include "DataStructures/wrappers.h"
#include "DataStructures/stack.h"
#include "DataStructures/queue.h"
#include "DataStructures/Graphs/graph.h"

bool print(const std::string& id);
int main() {
	

	std::string nodes[8] = { "A", "B", "C", "D" ,"E", "F", "G", "H" };


	std::string ANeighbors[4] = {"A", "D", "B", "H"};
	std::string BNeighbors[4] = {"E"};
	std::string CNeighbors[4] = {"B", "A"};
	std::string DNeighbors[4] = {"A", "B", "E", "G"};
	std::string ENeighbors[4] = { };
	std::string FNeighbors[4] = { "F", "D" };
	std::string GNeighbors[4] = { "H" };
	std::string HNeighbors[4] = { "H" };
	HashTable<std::string[4]> nodeDict{8};
	nodeDict.put("A", ANeighbors);
	nodeDict.put("B", BNeighbors);
	nodeDict.put("C", CNeighbors);
	nodeDict.put("D", DNeighbors);
	nodeDict.put("E", ENeighbors);
	nodeDict.put("F", FNeighbors);
	nodeDict.put("G", GNeighbors);
	nodeDict.put("H", HNeighbors);



	Graph<Int> graph = Graph<Int>(nodes, nodeDict, 1024);
	
	std::cout << graph.to_string() << std::endl;

	graph.swap_nodes("A", "D");
	std::cout << graph.to_string() << std::endl;


	std::string iNeighbors[4] = {"J", "K"};
	std::string iParents[2] = { "L", "A" };
	graph.insert("I", iNeighbors, iParents);
	std::cout << graph.to_string() << std::endl;

	SinglyLinkedList<String>* lst = new SinglyLinkedList<String>();

	graph.depth_first_search("F", lst);

	std::string arr[12];
	lst->to_array(arr);
	
	SinglyLinkedList<String>* lst2[8]{};
	SinglyLinkedList<String>* (&lst2reff)[8] = lst2;
	for (auto &lst : lst2) {
		lst = new SinglyLinkedList<String>;
	}
	std::string startIds[8] = { "A", "D" };
	std::string (& startIdsRef)[8] = nodes;


	graph.breadth_first_search(startIdsRef, lst2reff);




	std::string MDSids[4] = { "A", "D", "E",  "B"};
	std::string(&MDSidsRef)[4] = MDSids;

	SinglyLinkedList<String>* lst3[4]{};
	SinglyLinkedList<String>* (&lst3reff)[4] = lst3;
	graph.multi_directional_search(MDSidsRef, lst3reff);


	SinglyLinkedList<String>* lst4 = new SinglyLinkedList<String>();

	std::string projects[6] = { "A", "B", "C", "D", "E", "F" };
	std::string(&projectsRef)[6] = projects;

	std::string Achildren[2] = { "D" };
	std::string Bchildren[2] = { "D", "C"};
	std::string Cchildren[2] = { };
	std::string Dchildren[2] = { "C" };
	std::string Fchildren[2] = { "A", "B"};

	HashTable<std::string[2]> dependencies{ 6 };
	dependencies.put("A", Achildren);
	dependencies.put("B", Bchildren);
	dependencies.put("C", Cchildren);
	dependencies.put("D", Dchildren);
	dependencies.put("F", Fchildren);

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
