#pragma once
#include <string>
#include <iostream>
#include "lists.h"


template <typename T>
struct HashItem{

	std::string key;
	T* value;

	HashItem(std::string key, T &value) : key(key), value(&value) {}
	std::string to_string() {
		std::stringstream ss;
		ss << "key: " << key << "..." << "value: " << value->to_string();
		return ss.str();
	}
};


template <typename T>
class HashTable {

public:

	size_t size;
	size_t count;

private:

	SinglyLinkedList<HashItem<T>>** slots;
	SinglyLinkedList<size_t>* indices;

public:

	HashTable(size_t N=NULL) {
		
		size = N ? N : 256;
		count = 0;
		slots = new SinglyLinkedList<HashItem<T>> * [size]();
		indices = new SinglyLinkedList<size_t>();
	}


	size_t hash(std::string key) {

		size_t multiplier = 1;
		size_t hashValue = 0;

		for (short c : key) {
			hashValue += multiplier * c;
			multiplier++;
		}
		return hashValue % size;
	}


	void put(std::string key, T &value) {


		size_t hashValue = hash(key);
		HashItem<T> *item = new HashItem<T>(key, value);
		
		SinglyLinkedList<HashItem<T>> *lst = slots[hashValue];

		if (!lst)
		{
			count++;
			slots[hashValue] = new SinglyLinkedList<HashItem<T>>();
			slots[hashValue]->append(*item);
			indices->append(*(new size_t(hashValue)));
			return;
		}
		
		Node<HashItem<T>> *ptr = lst->head;
		while (ptr) {
			if (ptr->data->key == key) {
				ptr->data = item;
				return;
			}
			ptr = ptr->next;
		}
		lst->append(*item);
		count++;

		return;
	}


	T* get(std::string key) {

		size_t hashValue = hash(key);
		SinglyLinkedList<HashItem<T>>* lst = slots[hashValue];

		if (!lst) {
			return NULL;
		}

		Node<HashItem<T>>* ptr = lst->head;
		while (ptr) {
			if (ptr->data->key == key) {
				return ptr->data->value;
			}
		}
		return NULL;
	}


	void remove(std::string key) {

		size_t hashValue = hash(key);
		SinglyLinkedList<HashItem<T>> *lst = slots[hashValue];
		Node<HashItem<T>> *slow = lst->head, *fast = lst->head->next;

		// removing hashValue by value, and not by reference, thus cannot use indices->remove(hashValue). 
		if (*(indices->head->data) == hashValue) {
			Node<size_t> *temp = indices->head;
			indices->head = indices->head->next;
			delete(temp);
		}
		else {
			Node<size_t> *slow = indices->head, *fast = indices->head->next;
			while (fast) {
				if (*(fast->data) == hashValue) {
					slow->next = fast->next;
					fast = fast->next;
					if (*(indices->tail->data) == hashValue) {
						indices->tail = slow;
					}
					break;
				}
				slow = fast;
				fast = fast->next;
			}
		}


		if (slow->data->key == key) {
			count--;
			Node<HashItem<T>> *temp = lst->head->next;
			delete(lst->head);
			lst->head = temp;
			return;
		}

		while (fast) {
			if (fast->data->key == key) {
				count--;
				slow->next = fast->next;
				if (fast == lst->tail) {
					lst->tail = slow;
				}
				delete(fast);
				break;
			}
			return;
		}
	}


	std::string to_string() {

		std::stringstream ss;
		ss << "\n{ ---Hash Table--- \n";
		Node<size_t> *indexPtr = indices->head;
		while (indexPtr) {

			SinglyLinkedList<HashItem<T>>* listPtr = slots[*(indexPtr->data)];
			ss << listPtr->to_string();
		
			indexPtr = indexPtr->next;
		}
		ss << "count: " << count << ", size: " << size << "}";
		return ss.str();
	}
};