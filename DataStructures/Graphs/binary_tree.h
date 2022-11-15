#pragma once

#include "tree.h"
#include "binary_tree_node.h"

class BinaryTree : public Tree {

public:


	BinaryTree(const std::string& title = "", bool weighted = false);


	void pre_order_traversal(std::vector<std::string>& memo);


	void in_order_traversal(std::vector<std::string>& memo);


	void post_order_traversal(std::vector<std::string>& memo);

protected:


	virtual void create_node(const std::string& id, const bool weighted = false) override;


	virtual BinaryTreeNode* get_node(const std::string& id) const override;


	virtual BinaryTreeNode* get_node(String& id) const override;


	void is_valid_edge(const std::string& parent, const std::string& child) override;


	void is_valid_binary_tree_edge(const std::string& parent, const std::string& child);


	void validate_graph() override;


	virtual void set_root(TreeNode* root);
	

	void insert_check(const std::string& parent, const std::string& child) const override;


	void pre_order_traversal(String& currId, SmartList<String>* memo);


	void in_order_traversal(String& currId, SmartList<String>* memo);


	void post_order_traversal(String& currId, SmartList<String>* memo);

};