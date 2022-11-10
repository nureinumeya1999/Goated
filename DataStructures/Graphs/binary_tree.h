#pragma once

#include "tree.h"


class BinaryTreeNode : public TreeNode {
	BinaryTreeNode(String id, const bool weighted = false)
		: TreeNode(id, weighted) {
		this->set_type("BinaryTreeNode");
	}
};


class BinaryTree : public Tree {

public:

	BinaryTree(const std::string& title = "", size_t size = NULL, bool weighted = false)
		: Tree(title, weighted) {
		this->set_type("BinaryTree");
	}

protected:





};