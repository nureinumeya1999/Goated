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
	
	Tree(const std::string& title = "", size_t size = NULL, bool weighted = false)
		: Graph(title, weighted) {
		this->set_type("Tree");
	}

	std::string get_root() {
		return this->root->id.to_string();
	}

	using Graph::forest_depth_first_search;
	
protected:

	void is_valid_edge(const std::string& parent, const std::string& child) override {
		if (this->get_node(child)->parents->size == 1) {
			std::cerr << "TreeNodeError: Attempting to give parent <" <<
				parent << "> to child <" << child << "> which already has parent <"
				<< this->get_node(child)->parents->head->data->node->id.to_string() << ">." << std::endl;
			throw std::invalid_argument("Child node <" + child + "> already has a parent.");
		}
	}


	void validate_graph() override {
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
				this->root = root;
			}
		}
	}


	void create_node(const std::string& id, const bool weighted = false) override {
		this->count++;
		String* nodeId = new String(id);
		TreeNode* newGraphNode = new TreeNode(*nodeId, weighted);
		this->ids->append(*nodeId);
		this->nodes->put(id, *newGraphNode);
	}


	TreeNode* get_node(const std::string& id) const override {
		return static_cast<TreeNode*>(this->nodes->get(id)); }


	TreeNode* get_node(String& id) const override {
		return static_cast<TreeNode*>(this->nodes->get(id.to_string())); }


	std::string info() const override {
		std::stringstream ss;
		ss << "count: " << this->count << ", root: " << this->root->id.to_string();
		return ss.str();
	}

	
	void forest_depth_first_search(SinglyLinkedList<SinglyLinkedList<String>>* memo,
		callType func = nullptr) override {

		std::string start = this->root->id.to_string();
		SinglyLinkedList<String>* DFSmemo = new SinglyLinkedList<String>;
		this->depth_first_search(start, DFSmemo, func);
		memo->append(*DFSmemo);

		std::cout << "Forested depth first search on tree <" << this->graphId << "> finished. Returned with{\n"
			<< memo->to_string() << "\n}" << std::endl;
	}
};