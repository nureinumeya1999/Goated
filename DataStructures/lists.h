#pragma once 
#include "nodes.h"
#include <string>
#include <sstream>
#include <memory>
#include "wrappers.h"
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


	std::string to_string(bool formatted=true) const {

		Node<T> *ptr = head;
		std::stringstream ss;

		std::string start = formatted ? "[" : "<";
		std::string end = formatted ? "]" : ">";
		std::string sep = formatted ? ", " : " -> ";

		ss << start;
		while (ptr) {
			ss << ptr->data->to_string(formatted);
			if (ptr != this->tail) {
				ss << sep;
			}
			ptr = ptr->next;
		};
		ss << end;
		return ss.str();
	};


	bool contains(const T* const data) const {
		if (!head) {
			return false;
		}
		Node<T>* ptr = head;
		while (ptr) {
			if (ptr->data== data) {
				return true;
			}
			ptr = ptr->next;
		}
		return false;
	}


	void remove(const T* const data) {

		if (head->data == data) {
			Node<T> *temp = head;
			head = head->next;
			delete(temp);
			return;
		}
		Node<T> *slow = head, *fast = head->next;
		while (fast) {

			if (fast->data == data) {
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


	void extend(SinglyLinkedList<T>* other) {
		tail->next = other->head;
		size += other->size; 
	}

	
	SinglyLinkedList<String>* difference(SinglyLinkedList<T>* other) {

		SinglyLinkedList<String>* diff = new SinglyLinkedList<String>();
		Node<T>* ptr1 = head;
		while (ptr1) {
			Node<T>* ptr2 = other->head;
			while (ptr2) {
				if (ptr1->data->to_string() == ptr2->data->to_string()) {
					diff->append(*(new String(ptr1->data->to_string())));
				}
				ptr2 = ptr2->next;
			}
			ptr1 = ptr1->next;
		}
		return diff;
	}

	template<typename...kwargs>
	SinglyLinkedList<String>* difference(SinglyLinkedList<T>* other, kwargs...rest) {
		SinglyLinkedList<String>* diff = new SinglyLinkedList<String>();
		diff->extend(
			difference(rest...)
		);
		return diff;
	}


	SinglyLinkedList<T>* copy() {

		SinglyLinkedList<T>* lst = new SinglyLinkedList<T>();
		
		if (!head) {
			return lst;
		}

		Node<T>* ptr = head;
		Node<T>* new_ptr = new Node<T>(*ptr->data);
		lst->head = new_ptr;
		lst->tail = new_ptr;
		ptr = ptr->next;
		while (ptr) {
			lst->tail->next = new Node<T>(*ptr->data);
			lst->tail = lst->tail->next;
			ptr = ptr->next;
		}
	
		return lst;
		
	}

	template<size_t N>
	void to_array(std::string (&empty)[N]) {

		Node<T>* ptr = head;
		for (size_t i = 0; i < N; i++) {
			if (!ptr) { return; }
			empty[i] = ptr->data->to_string();
			ptr = ptr->next;
		}
		return;
	}

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
























