#pragma once
#include "tree_node.h"

class BinaryTreeNode : public TreeNode {

public:

	BinaryTreeNode(String id, const bool weighted = false);


	virtual Neighbor<GraphNode>* child1() const;


	virtual Neighbor<GraphNode>* child2() const;


	std::string to_string(bool formatted = true) const override;
};
