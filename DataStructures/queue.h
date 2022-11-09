#pragma once
#include "nodes.h"
#include <string>
#include <sstream>


template <typename T>
class Queue {

public:

	size_t size;
	Node<T>* head;
	Node<T>* tail;

public:

	Queue() : size(0), head(nullptr), tail(nullptr) {};

	void enqueue(T& data) {
		size++;
		Node<T>* newNode = new Node<T>(data);
		if (tail) {
			tail->next = newNode;
			tail = newNode;
			return;
		}

		head = newNode;
		tail = newNode;
		return;
	}

	T* dequeue() {
		if (!head) {
			return NULL;
		}
		Node<T>* temp = head;
		T* data = head->data;
		if (tail == head) {
			tail = tail->next;
		}
		head = head->next;
		size--;
		delete(temp);
		return data;
	}


	std::string to_string(bool formatted=true) {

		std::stringstream ss;
		ss << "FRONT( ";
		Node<T>* ptr = head;
		while (ptr) {
			ss << ptr->data->to_string();
			if (ptr != tail) {
				ss << " > ";
			}
			ptr = ptr->next;
		}
		ss << " )BACK\n";
		return ss.str();
	}


	bool is_empty() {
		if (head) {
			return false;
		}
		return true;
	}
};