#include "graph_node.h"


GraphNode::GraphNode(String id, const bool weighted)
	: id(id), weighted(weighted) {

	if (!weighted) {
		this->type = "GraphNode";
	}
	else {
		this->type = "WeightedGraphNode";
	}
	children = new SinglyLinkedList<Neighbor<GraphNode>>();
	parents = new SinglyLinkedList<Neighbor<GraphNode>>();
}


std::string GraphNode::to_string(bool formatted) const {

	std::stringstream ss;
	std::string sep = formatted ? "  " : " ~ ";

	ss << this->type << "[ id: " << id.to_string() << sep << "children: [";

	Node<Neighbor<GraphNode>>* childPtr = children->head;
	while (childPtr) {
		if (weighted) { ss << "("; }
		ss << childPtr->data->node->id.to_string();
		if (weighted) {
			ss << ", " << childPtr->data->weight << ")";
		}
		if (childPtr != children->tail) {
			ss << ", ";
		}
		childPtr = childPtr->next;
	}
	ss << "]" << sep << "parents: [";

	Node<Neighbor<GraphNode>>* parentPtr = parents->head;
	while (parentPtr) {
		if (weighted) { ss << "("; }
		ss << parentPtr->data->node->id.to_string();
		if (weighted) {
			ss << ", " << parentPtr->data->weight << ")";
		}
		if (parentPtr != parents->tail) {
			ss << ", ";
		}
		parentPtr = parentPtr->next;
	}
	ss << "] ]";

	return ss.str();
}


void GraphNode::set_type(const std::string& type) {
	this->type = type;
}
