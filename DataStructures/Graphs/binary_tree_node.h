#pragma once
#include "tree_node.h"

class BinaryTreeNode : public TreeNode {

public:

	BinaryTreeNode(String id, const bool weighted = false);


	Neighbor<GraphNode>* child1() const;


	Neighbor<GraphNode>* child2() const;


	std::string to_string(bool formatted = true) const override;
};
