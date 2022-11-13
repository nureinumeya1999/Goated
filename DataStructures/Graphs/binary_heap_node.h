#pragma once
#include "binary_tree_node.h"

class BinaryHeapNode : BinaryTreeNode {

public:

	double data;

public:

	BinaryHeapNode(String id, double data, const bool weighted = false);

	std::string to_string(bool formatted = true) const override;

};