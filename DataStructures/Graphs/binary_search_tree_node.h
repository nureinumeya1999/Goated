#pragma once
#include "binary_tree_node.h"


class BinarySearchTreeNode : public BinaryTreeNode {

public:

	double data;

public:

	BinarySearchTreeNode(String id, double data, const bool weighted = false);


	Neighbor<GraphNode>* child1() const override;


	Neighbor<GraphNode>* child2() const override;


	Neighbor<GraphNode>* lchild() const;


	Neighbor<GraphNode>* rchild() const;


	std::string to_string(bool formatted = true) const override;
};