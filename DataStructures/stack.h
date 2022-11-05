#pragma once
#include "nodes.h"
#include <string>
#include <sstream>

template <typename T>
class Stack {
public:

	size_t size;
	Node<T> *top;

public:

	Stack() :size(0), top(nullptr) {}

	void push(T &data) {

		size++;
		Node<T> *newNode = new Node<T>(data);

		if (!top) {
			top = newNode;
			return;
		}

		newNode->next = top;
		top = newNode;
		return;
	}

	
	T* pop() {

		Node<T>* tempNode = top;
		T* data = top->data;
		top = top->next;
		delete(tempNode);
		return data;
	}


	std::string to_string(bool formatted=true) {

		std::stringstream ss;
		ss << "\nTOP[ ";
		Node<T> *ptr = top;
		while (ptr) {
			ss << ptr->data->to_string();
			if (ptr->next) {
				ss << " || ";
			}
			ptr = ptr->next;
		}
		ss << " ]BOTTOM\n";
		return ss.str();
			
	}

};