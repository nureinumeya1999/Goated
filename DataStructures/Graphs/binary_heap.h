#pragma once
#include "binary_tree.h"
#include "binary_search_tree.h"

class BinaryHeap : public BinarySearchTree {

public:

	SinglyLinkedList<String>* heap;

public:

	
	BinaryHeap(const std::string& title = "", bool weighted = false);


	void float_index(size_t index);


	void sink_index(size_t index);


	void insert(const std::string& id, double data, double weight=NULL);


	void pop();


	void remove_node(const std::string& id) override;


	void update_node(const std::string& node, double data);


protected:

};