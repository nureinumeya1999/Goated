#include "binary_tree.h"

BinaryTree::BinaryTree(const std::string& title, bool weighted)
	: Tree(title, weighted) {
	if (weighted) {
		this->set_type("[Weighted]BinaryTree");
	}
	else {
		this->set_type("BinaryTree");
	}

}


void BinaryTree::pre_order_traversal(std::vector<std::string>& memo) {
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


void BinaryTree::in_order_traversal(std::vector<std::string>& memo) {
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


void BinaryTree::post_order_traversal(std::vector<std::string>& memo) {
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

void BinaryTree::create_node(const std::string& id, const bool weighted) {
	this->count++;
	String* nodeId = new String(id);
	BinaryTreeNode* newGraphNode = new BinaryTreeNode(*nodeId, weighted);
	this->ids->append(*nodeId);
	this->nodes->put(id, *newGraphNode);
}


BinaryTreeNode* BinaryTree::get_node(const std::string& id) const {
	return static_cast<BinaryTreeNode*>(this->nodes->get(id));
}


BinaryTreeNode* BinaryTree::get_node(String& id) const {
	return static_cast<BinaryTreeNode*>(this->nodes->get(id.to_string()));
}


void BinaryTree::is_valid_edge(const std::string& parent, const std::string& child) {
	this->is_valid_tree_edge(parent, child);
	this->is_valid_binary_tree_edge(parent, child);
}


void BinaryTree::is_valid_binary_tree_edge(const std::string& parent, const std::string& child) {
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


void BinaryTree::validate_graph() {
	this->validate_tree();
}



void BinaryTree::set_root(TreeNode* root) {
	this->root = static_cast<BinaryTreeNode*>(root);
}

void BinaryTree::insert_check(const std::string& parent, const std::string& child) const {
	if (get_node(child)) {
		std::cerr << "Insert Failed: Child node already exists." << std::endl;
		throw std::invalid_argument("Child node already exists.");
	}
	if (!get_node(parent)) {
		std::cerr << "Insert Failed: Parent node does not exist." << std::endl;
		throw std::invalid_argument(" Parent node does not exist.");
	}
	if (get_node(parent)->children->size == 2) {
		std::cerr << "Insert Failed: Parent node has two children." << std::endl;
		throw std::invalid_argument("Parent node has two children.");
	}
}


void BinaryTree::pre_order_traversal(String& currId, SinglyLinkedList<String>* memo) {

	BinaryTreeNode* currNode = this->get_node(currId);
	memo->append(currId);
	if (currNode->child1()) { pre_order_traversal(currNode->child1()->node->id, memo); }
	if (currNode->child2()) { pre_order_traversal(currNode->child2()->node->id, memo); }
}


void BinaryTree::in_order_traversal(String& currId, SinglyLinkedList<String>* memo) {

	BinaryTreeNode* currNode = this->get_node(currId);
	if (currNode->child1()) { in_order_traversal(currNode->child1()->node->id, memo); }
	memo->append(currId);
	if (currNode->child2()) { in_order_traversal(currNode->child2()->node->id, memo); }
}


void BinaryTree::post_order_traversal(String& currId, SinglyLinkedList<String>* memo) {

	BinaryTreeNode* currNode = this->get_node(currId);
	if (currNode->child1()) { post_order_traversal(currNode->child1()->node->id, memo); }
	if (currNode->child2()) { post_order_traversal(currNode->child2()->node->id, memo); }
	memo->append(currId);
}