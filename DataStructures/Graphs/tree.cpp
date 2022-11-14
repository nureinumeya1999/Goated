#include "tree.h"

Tree::Tree(const std::string& title, bool weighted)
	: Graph(title, weighted) {

	if (weighted) {
		this->set_type("[Weighted]Tree");
	}
	else {
		this->set_type("Tree");
	}
}


std::string Tree::get_root() {
	return this->root->id.to_string();
}


void Tree::insert(const std::string& parent, const std::string& child) {
	validate_weight(false);
	insert_check(parent, child);
	create_node(child, false);
	make_edge(parent, child, -1);
}


void Tree::insert(const std::string& parent, const std::string& child, double weight) {
	validate_weight(true);
	insert_check(parent, child);
	create_node(child, true);
	make_edge(parent, child, weight);
}


void Tree::remove_node(const std::string& nodeId) {

	
	GraphNode* node = get_node(nodeId);
	if (node->children->size != 0) {
		std::cerr << "Cannot remove tree node that has children." << std::endl;
	}
	else {
		if (this->root->id.to_string() == nodeId) {
			this->root = nullptr;
		}
		remove_node(node->id);
	}
}


void Tree::create_node(const std::string& id, const bool weighted) {
	this->count++;
	String* nodeId = new String(id);
	TreeNode* newGraphNode = new TreeNode(*nodeId, weighted);
	this->ids->append(*nodeId);
	this->nodes->put(id, *newGraphNode);
}


TreeNode* Tree::get_node(const std::string& id) const {
	return static_cast<TreeNode*>(this->nodes->get(id));
}


TreeNode* Tree::get_node(String& id) const {
	return static_cast<TreeNode*>(this->nodes->get(id.to_string()));
}


void Tree::is_valid_edge(const std::string& parent, const std::string& child) {
	is_valid_tree_edge(parent, child);
}


void Tree::is_valid_tree_edge(const std::string& parent, const std::string& child) {
	if (this->get_node(child)->parents->size == 1) {
		std::cerr << "TreeNodeError: Attempting to give parent <" <<
			parent << "> to child <" << child << "> which already has parent <"
			<< this->get_node(child)->parents->head->data->node->id.to_string() << ">." << std::endl;
		throw std::invalid_argument("Child node <" + child + "> already has a parent.");
	}
}


void Tree::validate_graph() {
	this->validate_tree();
}


void Tree::validate_tree() {
	SinglyLinkedList<String>* topSorted = new SinglyLinkedList<String>;
	this->topological_sort(topSorted);
	if (topSorted->is_empty()) {
		std::cerr << "Tree cannot be made; edges contain a cycle." << std::endl;
		throw std::invalid_argument("Tree cannot be made; edges contain a cycle.");
	}
	else {
		TreeNode* root = this->get_node(*topSorted->head->data);
		SinglyLinkedList<String>* dfs = new SinglyLinkedList<String>;
		this->depth_first_search(root->id.to_string(), dfs);
		if (dfs->size != this->count) {
			std::cerr << "Tree cannot be made, nodes aren't connected." << std::endl;
			throw std::invalid_argument("Tree cannot be made, nodes aren't connected.");
		}
		else {
			this->set_root(root);
		}
	}
}


void Tree::set_root(TreeNode* root) {
	this->root = root;
}


std::string Tree::info() const {
	std::stringstream ss;
	ss << "count: " << this->count << ", root: ";
	if (this->root) {
		ss << this->root->id.to_string();
	}
	else {
		ss << "NONE";
	}
	return ss.str();
}


void Tree::insert_check(const std::string& parent, const std::string& child) const {
	if (get_node(child)) {
		std::cerr << "Insert Failed: Child node already exists." << std::endl;
		throw std::invalid_argument("Child node already exists.");
	}
	if (!get_node(parent)) {
		std::cerr << "Insert Failed: Parent node does not exist." << std::endl;
		throw std::invalid_argument(" Parent node does not exist.");
	}
}


void Tree::forest_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo,
	callType func) {

	std::string start = this->root->id.to_string();
	SinglyLinkedList<String>* DFSmemo = new SinglyLinkedList<String>;
	this->depth_first_search(start, DFSmemo, func);
	memo->append(*DFSmemo);

}


void Tree::forest_post_order_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo) {

	SinglyLinkedList<String>* seen = new SinglyLinkedList<String>;
	SinglyLinkedList<String>* postDFSmemo = new SinglyLinkedList<String>;

	post_order_depth_first_search(this->root->id, postDFSmemo, seen);
	memo->append(*postDFSmemo);

}