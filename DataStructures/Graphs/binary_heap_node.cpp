#include "binary_heap_node.h"

BinaryHeapNode::BinaryHeapNode(String id, double data, size_t index) 
	: BinaryTreeNode(id), data(data), index(index) {
		this->set_type("BinaryHeapNode");
}


std::string BinaryHeapNode::to_string(bool formatted) const {

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

	ss << this->type << "[ {id: " << id.to_string()
		<< ", data: " << this->data << "}";


	ss << sep << "child1: [";
	if (child1) {
		if (weighted) { ss << "("; }
		ss << "{";
		ss << static_cast<BinaryHeapNode*>(child1->node)->id.to_string()
			<< ", " << static_cast<BinaryHeapNode*>(child1->node)->data;
		ss << "}";
		if (weighted) { if (child1) { ss << ", " << child1->weight; } ss << ")"; }
	}
	ss << "]";

	ss << sep << "child2: [";
	if (child2) {
		if (weighted) { ss << "("; }
		ss << "{";
		ss << static_cast<BinaryHeapNode*>(child2->node)->id.to_string()
			<< ", " << static_cast<BinaryHeapNode*>(child2->node)->data;
		ss << "}";
		if (weighted) { if (child2) { ss << ", " << child2->weight; } ss << ")"; }
	}
	ss << "]";

	ss << sep << "parent: [";
	DNode<Neighbor<GraphNode>>* parentPtr = parents->head;

	if (parentPtr) {
		if (weighted) { ss << "("; }
		ss << "{";
		ss << static_cast<BinaryHeapNode*>(parentPtr->data->node)->id.to_string()
			<< ", " << static_cast<BinaryHeapNode*>(parentPtr->data->node)->data;
		ss << "}";
		if (weighted) { if (parentPtr) { ss << ", " << parentPtr->data->weight; } ss << ")"; }
	}
	ss << "]";
	ss << " ]";
	return ss.str();
}
