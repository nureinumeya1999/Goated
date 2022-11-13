#include "binary_search_tree_node.h"



BinarySearchTreeNode::BinarySearchTreeNode(String id, double data, const bool weighted)
	: BinaryTreeNode(id, weighted) {
	this->set_type("BinarySearchTreeNode");
	this->data = data;
}


Neighbor<GraphNode>* BinarySearchTreeNode::lchild() const {
	if (this->child1() && \
		static_cast<BinarySearchTreeNode*>(this->child1()->node)->data < this->data) {
		return this->child1();
	}
	if (this->child2() && \
		static_cast<BinarySearchTreeNode*>(this->child1()->node)->data < this->data) {
		return this->child2();
	}
	return nullptr;
}


Neighbor<GraphNode>* BinarySearchTreeNode::BinarySearchTreeNode::rchild() const {
	if (this->child1() && \
		static_cast<BinarySearchTreeNode*>(this->child1()->node)->data >= this->data) {
		return this->child1();
	}
	if (this->child2() && \
		static_cast<BinarySearchTreeNode*>(this->child2()->node)->data >= this->data) {
		return this->child2();
	}
	return nullptr;
}


std::string BinarySearchTreeNode::to_string(bool formatted) const {

	std::stringstream ss;
	std::string sep = "  ";

	Neighbor<GraphNode>* lchild;
	if (this->lchild()) {
		lchild = this->lchild();
	}
	else {
		lchild = nullptr;
	}

	Neighbor<GraphNode>* rchild;
	if (this->rchild()) {
		rchild = this->rchild();
	}
	else {
		rchild = nullptr;
	}

	ss << this->type << "[ {id: " << id.to_string()
		<< ", data: " << this->data << "}";


	ss << sep << "lchild: [";
	if (lchild) {
		if (weighted) { ss << "("; }
		ss << "{";
		ss << static_cast<BinarySearchTreeNode*>(lchild->node)->id.to_string()
			<< ", " << static_cast<BinarySearchTreeNode*>(lchild->node)->data;
		ss << "}";
		if (weighted) { if (lchild) { ss << ", " << lchild->weight; } ss << ")"; }
	}
	ss << "]";

	ss << sep << "rchild: [";
	if (rchild) {
		if (weighted) { ss << "("; }
		ss << "{";
		ss << static_cast<BinarySearchTreeNode*>(rchild->node)->id.to_string()
			<< ", " << static_cast<BinarySearchTreeNode*>(rchild->node)->data;
		ss << "}";
		if (weighted) { if (rchild) { ss << ", " << rchild->weight; } ss << ")"; }
	}
	ss << "]";

	ss << sep << "parent: [";
	Node<Neighbor<GraphNode>>* parentPtr = parents->head;

	if (parentPtr) {
		if (weighted) { ss << "("; }
		ss << "{";
		ss << static_cast<BinarySearchTreeNode*>(parentPtr->data->node)->id.to_string()
			<< ", " << static_cast<BinarySearchTreeNode*>(parentPtr->data->node)->data;
		ss << "}";
		if (weighted) { if (parentPtr) { ss << ", " << parentPtr->data->weight; } ss << ")"; }
	}
	ss << "]";
	ss << " ]";
	return ss.str();
}