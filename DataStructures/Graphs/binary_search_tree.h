#pragma once
#include "binary_tree.h"

class BinarySearchTreeNode : public BinaryTreeNode {

public:

	double data; 

public:

	BinarySearchTreeNode(String id, double data, const bool weighted = false)
		: BinaryTreeNode(id, weighted) {
		this->set_type("BinarySearchTreeNode");
		this->data = data;
	}

	Neighbor<GraphNode>* lchild() const {
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


	Neighbor<GraphNode>* rchild() const {
		if (this->child1() && \
			static_cast<BinarySearchTreeNode*>(this->child1()->node)->data >= this->data) {
			return this->child1();
		}
		if (this->child2() && \
			static_cast<BinarySearchTreeNode*>(this->child1()->node)->data >= this->data) {
			return this->child2();
		}
		return nullptr;
	}


	std::string to_string(bool formatted = true) const override {

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
};


class BinarySearchTree : public BinaryTree {

public:

	BinarySearchTree(const std::string& title = "", bool weighted = false)
		: BinaryTree(title, weighted) {
		if (weighted) {
			this->set_type("WeightedBinarySearchTree");
		}
		else {
			this->set_type("BinarySearchTree");
		}
	}


	void initialize(std::pair<std::string, double>(&nodes)[],
		std::string(&edges)[], size_t size = NULL) {

		if (this->hasInitialized) {
			std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
			return;
		}
		std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
		size_t N = size ? size : 256;
		init_graph(N);
		init_nodes(nodes, false);
		init_edges(edges);
		validate_graph();
		this->hasInitialized = true;
		std::cout << this->to_string() << std::endl;
	}


	void initialize(std::vector<std::pair<std::string, double>>& nodes,
		std::vector<std::string>& edges, size_t size = NULL) {

		if (this->hasInitialized) {
			std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
			return;
		}
		std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
		size_t N = size ? size : nodes.size();
		init_graph(N);
		init_nodes(nodes, false);
		init_edges(edges);
		validate_graph();
		this->hasInitialized = true;
		std::cout << this->to_string() << std::endl;
	}


	void initialize(std::vector<std::pair<std::string, double>>& nodes,
		weighted_edge(&edges)[], size_t size = NULL) {

		if (this->hasInitialized) {
			std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
			return;
		}
		std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
		size_t N = size ? size : nodes.size();
		init_graph(N);
		init_nodes(nodes, true);
		init_edges(edges);
		validate_graph();
		this->hasInitialized = true;
		std::cout << this->to_string() << std::endl;
	}


	void initialize(std::pair<std::string, double>(&nodes)[],
		weighted_edge(&edges)[], size_t size = NULL) {

		if (this->hasInitialized) {
			std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
			return;
		}
		std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
		size_t N = size ? size : 256;
		init_graph(N);
		init_nodes(nodes, true);
		init_edges(edges);
		validate_graph();
		this->hasInitialized = true;
		std::cout << this->to_string() << std::endl;
	}

protected:

	virtual std::string info() const override {
		std::stringstream ss;
		ss << "count: " << this->count << ", root: {id: " << this->root->id.to_string()
			<< ", data: " << static_cast<BinarySearchTreeNode*>(this->root)->data
			<< "}";
		return ss.str();
	}

	void create_node(const std::string& id, double data, const bool weighted = false) {
		this->count++;
		String* nodeId = new String(id);
		BinarySearchTreeNode* newGraphNode = new BinarySearchTreeNode(*nodeId, data, weighted);
		this->ids->append(*nodeId);
		this->nodes->put(id, *newGraphNode);
	}


	void init_nodes(std::vector<std::pair<std::string, double>>& nodes, 
		const bool weighted = false) {

		for (int i = 0; i < nodes.size(); i++) {
			std::string nodeId = std::get<0>(nodes[i]);
			double data = std::get<1>(nodes[i]);
			if (!this->get_node(nodeId)) {
				this->create_node(nodeId, data, weighted);
			}
		}
	}


	void init_nodes(std::pair<std::string, double>(&nodes)[], 
		const bool weighted = false) {

		std::pair<std::string, double>* ptr = nodes;
		while (std::get<0>(*ptr) != "") {
			std::string nodeId = std::get<0>(*ptr);
			double data = std::get<1>(*ptr);
			if (!this->get_node(nodeId)) {
				this->create_node(nodeId, data, weighted);
			}
			ptr++;
		}
	}
};