#pragma once

#include "tree.h"


class BinaryTreeNode : public TreeNode {


public: 

	BinaryTreeNode(String id, const bool weighted = false)
		: TreeNode(id, weighted) {
		this->set_type("BinaryTreeNode");
	}

	Neighbor<GraphNode>* child1() const {
		if (this->children->size > 0) {
			return this->children->head->data;
		}
		else {
			return nullptr;
		}
	}

	Neighbor<GraphNode>* child2() const {
		if (this->children->size > 1) {
			return this->children->head->next->data;
		}
		else {
			return nullptr;
		}
	}


	std::string to_string(bool formatted = true) const override {

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
};


class BinaryTree : public Tree {

public:

	BinaryTree(const std::string& title = "", bool weighted = false)
		: Tree(title, weighted) {
		if (weighted) {
			this->set_type("WeightedBinaryTree");
		}
		else {
			this->set_type("BinaryTree");
		}
		
	}


	void pre_order_traversal(std::vector<std::string>& memo) {
		std::cout << "\nBeginning pre-order traversal..." << std::endl;
		SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;
		this->pre_order_traversal(this->root->id, memoList);
		Node<String>* ptr = memoList->head;
		while (ptr) {
			memo.push_back(ptr->data->to_string());
			ptr = ptr->next;
		}
		std::cout << "Pre-order traversal on " << this->type << " <" << this->graphId
			<< "> finished. Returned with{\n"
			<< memoList->to_string() << "\n}\n" << std::endl;
	}


	void in_order_traversal(std::vector<std::string>& memo) {
		std::cout << "\nBeginning in-order traversal..." << std::endl;
		SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;
		this->in_order_traversal(this->root->id, memoList);
		Node<String>* ptr = memoList->head;
		while (ptr) {
			memo.push_back(ptr->data->to_string());
			ptr = ptr->next;
		}
		std::cout << "In-order traversal on " << this->type << " <" << this->graphId
			<< "> finished. Returned with{\n"
			<< memoList->to_string() << "\n}\n" << std::endl;
	}


	void post_order_traversal(std::vector<std::string>& memo) {
		std::cout << "\nBeginning post-order traversal..." << std::endl;
		SinglyLinkedList<String>* memoList = new SinglyLinkedList<String>;
		this->post_order_traversal(this->root->id, memoList);
		Node<String>* ptr = memoList->head;
		while (ptr) {
			memo.push_back(ptr->data->to_string());
			ptr = ptr->next;
		}
		std::cout << "Post-order traversal on " << this->type << " <" << this->graphId
			<< "> finished. Returned with{\n"
			<< memoList->to_string() << "\n}\n" << std::endl;
	}

protected:

	void is_valid_edge(const std::string& parent, const std::string& child) override {
		this->is_valid_tree_edge(parent, child);
		this->is_valid_binary_tree_edge(parent, child);
	}


	void is_valid_binary_tree_edge(const std::string& parent, const std::string& child) {
		BinaryTreeNode* parentNode = this->get_node(parent);
		if (parentNode->children->size == 2) {
			std::cerr << "BinaryTreeNodeError: Attempting to give child <" <<
				child << "> to parent <" << parent << "> which already has two children <"
				<< parentNode->children->head->data->node->id.to_string() << ">, <"
				<< parentNode->children->head->next->data->node->id.to_string()
				<< ">. " << std::endl;
			throw std::invalid_argument("Parent node <" + parent + "> already has two children.");
		}
	}


	void validate_graph() override {
		this->validate_tree();
	}

	void create_node(const std::string& id, const bool weighted = false) override {
		this->count++;
		String* nodeId = new String(id);
		BinaryTreeNode* newGraphNode = new BinaryTreeNode(*nodeId, weighted);
		this->ids->append(*nodeId);
		this->nodes->put(id, *newGraphNode);
	}


	BinaryTreeNode* get_node(const std::string& id) const override {
		return static_cast<BinaryTreeNode*>(this->nodes->get(id));}


	BinaryTreeNode* get_node(String& id) const override {
		return static_cast<BinaryTreeNode*>(this->nodes->get(id.to_string()));}


	void pre_order_traversal(String& currId, SinglyLinkedList<String>* memo) {
		
		BinaryTreeNode* currNode = this->get_node(currId);
		memo->append(currId);
		if (currNode->child1()) { pre_order_traversal(currNode->child1()->node->id, memo); }
		if (currNode->child2()) { pre_order_traversal(currNode->child2()->node->id, memo); }
	}


	void in_order_traversal(String& currId, SinglyLinkedList<String>* memo) {

		BinaryTreeNode* currNode = this->get_node(currId);
		if (currNode->child1()) { in_order_traversal(currNode->child1()->node->id, memo); }
		memo->append(currId);
		if (currNode->child2()) { in_order_traversal(currNode->child2()->node->id, memo); }
	}


	void post_order_traversal(String& currId, SinglyLinkedList<String>* memo) {

		BinaryTreeNode* currNode = this->get_node(currId);
		if (currNode->child1()) { post_order_traversal(currNode->child1()->node->id, memo); }
		if (currNode->child2()) { post_order_traversal(currNode->child2()->node->id, memo); }
		memo->append(currId);
	}


};