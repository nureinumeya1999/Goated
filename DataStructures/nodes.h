#pragma once
#include <memory>

template <typename T> 
class Node {

public:
	T* data;
	Node<T> *next;

public:
	Node(T &data) {
		this->data = &data;
		this->next = nullptr;
	};

	
};

template <typename T>
class DNode {

public: 
	T* data;
	DNode<T> *next, *prev;

public:
	DNode(T &data) {
		this->data = &data;
		this->next = nullptr;
		this->prev = nullptr;
	};
};