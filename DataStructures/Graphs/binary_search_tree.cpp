#include "binary_search_tree.h"

BinarySearchTree::BinarySearchTree(const std::string& title, bool weighted)
	: BinaryTree(title, weighted) {
	if (weighted) {
		this->set_type("[Weighted]BinarySearchTree");
	}
	else {
		this->set_type("BinarySearchTree");
	}
}


void BinarySearchTree::initialize(size_t size) {
	BinaryTree::initialize(size);
};


void BinarySearchTree::initialize(std::pair<std::string, double>(&nodes)[], std::string(&edges)[], size_t size) {

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


void BinarySearchTree::initialize(std::vector<std::pair<std::string, double>>& nodes, std::vector<std::string>& edges, size_t size) {

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


void BinarySearchTree::initialize(std::vector<std::pair<std::string, double>>& nodes, weighted_edge(&edges)[], size_t size) {

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


void BinarySearchTree::initialize(std::pair<std::string, double>(&nodes)[], weighted_edge(&edges)[], size_t size) {

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


void BinarySearchTree::insert(std::pair<std::string, double> node, double weight) {

	if (this->weighted && weight < 0) {
		std::cerr << "Weight must be positive" << std::endl;
		return;
	}
	insert(std::get<0>(node), std::get<1>(node), weight);

}


void BinarySearchTree::insert(const std::string& id, double data, double weight) {

	if (this->weighted && weight < 0) {
		std::cerr << "Weight must be positive" << std::endl;
		return;
	}

	create_node(id, data, this->weighted);

	BinarySearchTreeNode* insert_node = get_node(id);

	if (!this->root) {
		this->set_root(get_node(id));
		return;
	}

	BinarySearchTreeNode* curr = static_cast<BinarySearchTreeNode*>(this->root);
	while (true) {
		if (insert_node->data < curr->data) {
			if (curr->lchild()) {
				curr = static_cast<BinarySearchTreeNode*>(curr->lchild()->node);
			}
			else {
				this->make_edge(curr->id.to_string(), insert_node->id.to_string(), weight);
				return;
			}
		}
		else {
			if (curr->rchild()) {
				curr = static_cast<BinarySearchTreeNode*>(curr->rchild()->node);
			}
			else {
				this->make_edge(curr->id.to_string(), insert_node->id.to_string(), weight);
				return;
			}
		}
	}
}




std::string BinarySearchTree::info() const {
	std::stringstream ss;
	ss << "count: " << this->count << ", root: ";
	if (this->root) {
		ss << "{id: " << this->root->id.to_string()
			<< ", data: " << static_cast<BinarySearchTreeNode*>(this->root)->data
			<< "}";
	}
	else {
		ss << "NONE";
	}

	return ss.str();
}

void BinarySearchTree::create_node(const std::string& id, double data, const bool weighted) {
	this->count++;
	String* nodeId = new String(id);
	BinarySearchTreeNode* newGraphNode = new BinarySearchTreeNode(*nodeId, data, weighted);
	this->ids->append(*nodeId);
	this->nodes->put(id, *newGraphNode);
}


BinarySearchTreeNode* BinarySearchTree::get_node(const std::string& id) const {
	return static_cast<BinarySearchTreeNode*>(this->nodes->get(id));
}


BinarySearchTreeNode* BinarySearchTree::get_node(String& id) const {
	return static_cast<BinarySearchTreeNode*>(this->nodes->get(id.to_string()));
}


void BinarySearchTree::init_nodes(std::vector<std::pair<std::string, double>>& nodes, const bool weighted) {

	for (int i = 0; i < nodes.size(); i++) {
		std::string nodeId = std::get<0>(nodes[i]);
		double data = std::get<1>(nodes[i]);
		if (!this->get_node(nodeId)) {
			this->create_node(nodeId, data, weighted);
		}
	}
}


void BinarySearchTree::init_nodes(std::pair<std::string, double>(&nodes)[], const bool weighted) {

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

void BinarySearchTree::set_root(TreeNode* root) {
	this->root = static_cast<BinarySearchTreeNode*>(root);
}


void BinarySearchTree::is_valid_edge(const std::string& parent, const std::string& child) {
	is_valid_binary_tree_edge(parent, child);
	is_valid_binary_search_tree_edge(parent, child);
}


void BinarySearchTree::is_valid_binary_search_tree_edge(const std::string& parent, const std::string& child) {
	BinarySearchTreeNode* parentNode = get_node(parent);
	BinarySearchTreeNode* childNode = get_node(child);

	if (parentNode->lchild()) {
		if (childNode->data < parentNode->data) {
			std::cerr << "Cannot add left child <" << child << "> to <" << parent <<
				"> which already has left child <" << parentNode->lchild()->id->to_string() << ">." << std::endl;
			throw std::invalid_argument("Parent node cannot have 2 left children.");
			return;
		}
	}

	if (parentNode->rchild()) {
		if (childNode->data >= parentNode->data) {
			std::cerr << "Cannot add right child <" << child << "> to <" << parent <<
				"> which already has right child <" << parentNode->rchild()->id->to_string() << ">." << std::endl;
			throw std::invalid_argument("Parent node cannot have 2 right children.");
			return;
		}
	}
}


void BinarySearchTree::validate_graph() {

	this->validate_tree();

	if (!this->root) {
		return;
	}
	else {
		this->validate_binary_search_tree();
	}
	
}


void BinarySearchTree::validate_binary_search_tree() {
	SinglyLinkedList<String>* memo = new SinglyLinkedList<String>;
	this->in_order_traversal(this->root->id, memo);

	bool isSorted = true;
	Node<String>* curr = memo->head;
	while (curr) {
		if (curr->next) {
			if (get_node(*curr->data)->data >= get_node(*curr->next->data)->data) {
				isSorted = false;
				break;
			}
		}
		curr = curr->next;
	}

	if (!isSorted) {
		std::cerr << "Binary Search Tree property not satisfied" << std::endl;
		throw std::invalid_argument("Binary Search Tree property not satisfied");
	}
}
