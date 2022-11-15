#pragma once
#include "graph.h"
#include "tree_node.h"

class Tree : public Graph {

public:

	TreeNode* root = nullptr;

public:
	
	using Graph::remove_node;
	using Graph::forest_depth_first_search;
	using Graph::forest_post_order_depth_first_search;


	Tree(const std::string& title = "", bool weighted = false);


	std::string get_root();


	void insert(const std::string& parent, const std::string& child) override;


	void insert(const std::string& parent, const std::string& child, double weight) override;


	virtual void remove_node(const std::string& nodeId) override;


protected:


	using Graph::insert;
	using Graph::make_edge;
	using Graph::remove_edge;


	virtual void create_node(const std::string& id, const bool weighted = false) override;


	virtual TreeNode* get_node(const std::string& id) const override;


	virtual TreeNode* get_node(String& id) const override;


	void is_valid_edge(const std::string& parent, const std::string& child) override;


	void is_valid_tree_edge(const std::string& parent, const std::string& child);


	virtual void validate_graph() override;


	void validate_tree();


	virtual void set_root(TreeNode* root);


	virtual std::string info() const override;


	virtual void insert_check(const std::string& parent, const std::string& child) const;

	
	void forest_depth_first_search(SmartList<SmartList<String>>* memo, callType func = nullptr) override;


	void forest_post_order_depth_first_search(SmartList<SmartList<String>>* memo) override;
};