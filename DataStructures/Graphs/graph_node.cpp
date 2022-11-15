#include "graph_node.h"


GraphNode::GraphNode(String id, const bool weighted)
	: id(id), weighted(weighted) {

	if (!weighted) {
		this->type = "GraphNode";
	}
	else {
		this->type = "WeightedGraphNode";
	}
	children = new SmartList<Neighbor<GraphNode>>();
	parents = new SmartList<Neighbor<GraphNode>>();
}


std::string GraphNode::to_string() const {

	std::stringstream ss;
	std::string sep = "  ";

	ss << this->type << "[ id: " << id.to_string() << sep << "children: [";

	DNode<Neighbor<GraphNode>>* childPtr = children->head;
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

	DNode<Neighbor<GraphNode>>* parentPtr = parents->head;
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
