#pragma once
#include "binary_search_tree_node.h"
#include "binary_tree_node.h"

class BinaryHeapNode : public BinaryTreeNode {

public:

	double data;
	size_t index = - 1;

public:

	BinaryHeapNode(String id, double data, size_t index);

	std::string to_string(bool formatted = true) const override;

};