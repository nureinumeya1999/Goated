#pragma once
#include "../lists.h"
#include <string>

template<typename T>
struct Neighbor {
	T* node;
	double weight;
	String* id;

	Neighbor(T* node, double weight) {
		this->node = node;
		this->weight = weight;
		this->id = &node->id;
	}
	String get_id() {
		return *id;
	}
};

class GraphNode {

public:

	std::string type = "GraphNode";
	String id;
	bool weighted;
	SinglyLinkedList<Neighbor<GraphNode>>* children;
	SinglyLinkedList<Neighbor<GraphNode>>* parents;

public:

	GraphNode(String id, const bool weighted = false);

	virtual std::string to_string(bool formatted = true) const;

protected:

	void set_type(const std::string& type);
};