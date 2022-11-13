#pragma once
#include "binary_tree.h"
#include "binary_search_tree_node.h"

class BinarySearchTree : public BinaryTree {

public:

	BinarySearchTree(const std::string& title = "", bool weighted = false);


	using BinaryTree::initialize;


	void initialize(std::pair<std::string, double>(&nodes)[], std::string(&edges)[], size_t size = NULL);


	void initialize(std::vector<std::pair<std::string, double>>& nodes, std::vector<std::string>& edges, size_t size = NULL);


	void initialize(std::vector<std::pair<std::string, double>>& nodes, weighted_edge(&edges)[], size_t size = NULL);


	void initialize(std::pair<std::string, double>(&nodes)[], weighted_edge(&edges)[], size_t size = NULL);


	void insert(std::pair<std::string, double> node, double weight = -1);


protected:
	
	using Tree::insert;
	using Tree::swap_nodes;


	virtual std::string info() const override;


	void create_node(const std::string& id, double data, const bool weighted = false);


	BinarySearchTreeNode* get_node(const std::string& id) const override;


	BinarySearchTreeNode* get_node(String& id) const override;


	void init_nodes(std::vector<std::pair<std::string, double>>& nodes, const bool weighted = false);


	void init_nodes(std::pair<std::string, double>(&nodes)[], const bool weighted = false);


	virtual void set_root(TreeNode* root);
};