#pragma once
#include "graph.h"


class TreeNode : public GraphNode {

public:

	TreeNode(String id, const bool weighted = false) 
		: GraphNode(id, weighted) {
		this->set_type("TreeNode");
	}

};


class Tree : public Graph {

public:

	TreeNode* root = nullptr;

public:
	
	Tree(const std::string& title = "", bool weighted = false)
		: Graph(title, weighted) {
		this->set_type("Tree");
	}

	std::string get_root() {
		return this->root->id.to_string();
	}

	using Graph::forest_depth_first_search;
	using Graph::forest_post_order_depth_first_search;

	
	void insert(const std::string& parent, const std::string& child) override {
		validate_weight(false);
		insert_check(parent, child);
		create_node(child, false);
		make_edge(parent, child, -1);
	}


	void insert(const std::string& parent, const std::string& child, double weight) override {
		validate_weight(true);
		insert_check(parent, child);
		create_node(child, true);
		make_edge(parent, child, weight);
	}


protected:

	using Graph::insert;
	using Graph::make_edge;


	virtual void create_node(const std::string& id, const bool weighted = false) override {
		this->count++;
		String* nodeId = new String(id);
		TreeNode* newGraphNode = new TreeNode(*nodeId, weighted);
		this->ids->append(*nodeId);
		this->nodes->put(id, *newGraphNode);
	}


	virtual TreeNode* get_node(const std::string& id) const override {
		return static_cast<TreeNode*>(this->nodes->get(id)); }


	virtual TreeNode* get_node(String& id) const override {
		return static_cast<TreeNode*>(this->nodes->get(id.to_string())); }


	void is_valid_edge(const std::string& parent, const std::string& child) override {
		is_valid_tree_edge(parent, child);
	}


	void is_valid_tree_edge(const std::string& parent, const std::string& child) {
		if (this->get_node(child)->parents->size == 1) {
			std::cerr << "TreeNodeError: Attempting to give parent <" <<
				parent << "> to child <" << child << "> which already has parent <"
				<< this->get_node(child)->parents->head->data->node->id.to_string() << ">." << std::endl;
			throw std::invalid_argument("Child node <" + child + "> already has a parent.");
		}
	}


	virtual void validate_graph() override {
		this->validate_tree();
	}

	void validate_tree() {
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

	virtual void set_root(TreeNode* root) {
		this->root = root;
	}



	virtual std::string info() const override {
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


	virtual void insert_check(const std::string& parent, const std::string& child) const {
		if (get_node(child)) {
			std::cerr << "Insert Failed: Child node already exists." << std::endl;
			throw std::invalid_argument("Child node already exists.");
		}
		if (!get_node(parent)) {
			std::cerr << "Insert Failed: Parent node does not exist." << std::endl;
			throw std::invalid_argument(" Parent node does not exist.");
		}
	}
	
	void forest_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo,
		callType func = nullptr) override {

		std::string start = this->root->id.to_string();
		SinglyLinkedList<String>* DFSmemo = new SinglyLinkedList<String>;
		this->depth_first_search(start, DFSmemo, func);
		memo->append(*DFSmemo);

	}

	void forest_post_order_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo) override {

		SinglyLinkedList<String>* seen = new SinglyLinkedList<String>;
		SinglyLinkedList<String>* postDFSmemo = new SinglyLinkedList<String>;

		post_order_depth_first_search(this->root->id, postDFSmemo, seen);
		memo->append(*postDFSmemo);

	}
};