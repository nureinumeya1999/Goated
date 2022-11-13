#include "binary_tree_node.h"

BinaryTreeNode::BinaryTreeNode(String id, const bool weighted)
	: TreeNode(id, weighted) {
	this->set_type("BinaryTreeNode");
}


Neighbor<GraphNode>* BinaryTreeNode::child1() const {
	if (this->children->size > 0) {
		return this->children->head->data;
	}
	else {
		return nullptr;
	}
}


Neighbor<GraphNode>* BinaryTreeNode::child2() const {
	if (this->children->size > 1) {
		return this->children->head->next->data;
	}
	else {
		return nullptr;
	}
}


std::string BinaryTreeNode::to_string(bool formatted) const {

	std::stringstream ss;
	std::string sep = "  ";

	Neighbor<GraphNode>* child1;
	if (this->child1()) {
		child1 = this->child1();
	}
	else {
		child1 = nullptr;
	}

	Neighbor<GraphNode>* child2;
	if (this->child2()) {
		child2 = this->child2();
	}
	else {
		child2 = nullptr;
	}


	ss << this->type << "[ id: " << id.to_string();

	ss << sep << "child1: [";
	if (child1) {
		if (weighted) { ss << "("; }
		ss << static_cast<BinaryTreeNode*>(child1->node)->id.to_string();
		if (weighted) { if (child1) { ss << ", " << child1->weight; } ss << ")"; }
	}
	ss << "]";

	ss << sep << "child2: [";
	if (child2) {
		if (weighted) { ss << "("; }
		ss << static_cast<BinaryTreeNode*>(child2->node)->id.to_string();
		if (weighted) { if (child2) { ss << ", " << child2->weight; } ss << ")"; }
	}
	ss << "]";
	ss << sep << "parent: [";

	Node<Neighbor<GraphNode>>* parentPtr = parents->head;

	if (parentPtr) {
		if (weighted) { ss << "("; }
		ss << parentPtr->data->node->id.to_string();
		if (weighted) {
			ss << ", " << parentPtr->data->weight << ")";
		}
	}
	ss << "]";
	ss << " ]";
	return ss.str();
}