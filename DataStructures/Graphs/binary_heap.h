#pragma once
#include "binary_search_tree.h"
#include "binary_heap_node.h"

typedef double(*priorityType)(double, double);

double min_priority(double first, double second);


class BinaryHeap : public BinaryTree {

public:

	SmartList<String>* heap;
	priorityType priority;

public:

	String priority_arg(String id1, String id2) const {
		BinaryHeapNode* node1 = get_node(id1);
		BinaryHeapNode* node2 = get_node(id2);
		if (priority(node1->data, node2->data) == node1->data) {
			return id1;
		}
		else {
			return id2;
		}
	}
	

	BinaryHeap(const std::string& title = "", priorityType priority = min_priority);


	void initialize(size_t size = NULL) override;


	void initialize(std::pair<std::string, double>(&nodes)[], std::string(&edges)[], size_t size = NULL);


	void initialize(std::vector<std::pair<std::string, double>>& nodes, std::vector<std::string>& edges, size_t size = NULL);

	
	void float_index(size_t index);


	void sink_index(size_t index);


	void insert(std::pair<std::string, double> (&nodes)[]);


	void insert(std::vector<std::pair<std::string, double>>& nodes);


	void insert(std::pair<std::string, double>& node);


	void insert(const std::string& id, double data);


	std::pair<String, Double>* pop();


	void sort(SmartList<Pair<String, Double>>* memo);


	void update_node(const std::string& node, double data);


protected:

	using BinaryTree::swap_nodes;


	std::string info() const override;


	void create_node(const std::string& id, double data, size_t index);


	BinaryHeapNode* get_node(const std::string& id) const override;


	BinaryHeapNode* get_node(String& id) const override;


	void init_nodes(std::vector<std::pair<std::string, double>>& nodes);


	void init_nodes(std::pair<std::string, double>(&nodes)[]);


	void is_valid_edge(const std::string& parent, const std::string& child) override;


	void is_valid_heap_edge(const std::string& parent, const std::string& child);


	void validate_graph() override;


	void validate_heap();


	void swap_nodes(String& id1, String& id2) override;

};