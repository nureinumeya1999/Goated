#pragma once 
#include "nodes.h"
#include <string>
#include <sstream>
#include <memory>

template <typename T>
class SinglyLinkedList {

public:

	Node<T> *head, *tail;
	size_t size;

public:

	SinglyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

	void append(T &data) {

		Node<T> *newNode = new Node<T>(data);
		size++;

		if (!head) {
			head = newNode;
			tail = newNode;
			return;
		}

		tail->next = newNode;
		tail = tail->next;
	
		return;
	};


	std::string to_string() {

		Node<T> *ptr = head;
		std::stringstream ss;

		ss << "<";
		while (ptr) {
			ss << ptr->data->to_string();
			if (ptr != this->tail) {
				ss << " -> ";
			}
			ptr = ptr->next;
		};
		ss << ">\n";
		return ss.str();
	};


	void remove(T &data) {

		if (head->data == &data) {
			Node<T> *temp = head;
			head = head->next;
			delete(temp);

			return;
		}

		Node<T> *slow = head, *fast = head->next;
		while (fast) {

			if (fast->data == &data) {

				size--;
				slow->next = fast->next;
				if (fast == tail) {
					tail = slow;
				}
				delete(fast);
				return;
			}
			else {
				slow = fast;
				fast = fast->next;
			}
		}
		return;



	};

};


template <typename T>
class DoublyLinkedList {

public:

	DNode<T> *head, *tail;
	size_t size;

public:

	DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}


	void append(T &data) {

		DNode<T> *newNode = new DNode<T>(data);
		size++;

		if (!head) {
			head = newNode;
			tail = newNode;
			return;
		}

		tail->next = newNode;
		newNode->prev = tail;
		tail = tail->next;
		return;
	}


	std::string to_string() {

		DNode<T> *ptr = head;
		std::stringstream ss;

		ss << "<";
		while (ptr) {
			ss << ptr->data->to_string();
			if (ptr != this->tail) {
				ss << " <-> ";
			}
			ptr = ptr->next;
		};
		ss << ">\n";
		return ss.str();
	};


	void remove(T &data) {

		if (head->data == &data) {
			head = head->next;
			return;
		}

		DNode<T> *slow = head, *fast = head->next;
		while (fast->next) {

			if (fast->data == &data) {
				size--;
				slow->next = fast->next;
				if (fast == tail) {
					tail = slow;
				}
				else {
					fast->next->prev = slow;
				}
				delete(fast);
				return;
			}
			else {
				slow = fast;
				fast = fast->next;
			}
		}
		return;

	};

};
























