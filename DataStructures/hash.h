#pragma once
#include <string>
#include <iostream>
#include "lists.h"
#include "wrappers.h"

template <typename T>
struct HashItem{

	std::string key;
	T* value;

	HashItem(const std::string& key, T &value) : key(key), value(&value) {}


	std::string to_string(bool formatted=true) const {
		std::string sep = formatted ? ", " : "...";
		std::stringstream ss;
		ss << "key: " << key << sep << "value: " << value->to_string(formatted);
		return ss.str();
	}
};


template <typename T>
class HashTable {

public:

	size_t size;
	size_t count;

	SinglyLinkedList<HashItem<T>>** slots;
	SinglyLinkedList<size_t>* indices;

public:

	HashTable(const size_t N=NULL) {
		
		size = N ? N : 256;
		count = 0;
		slots = new SinglyLinkedList<HashItem<T>> * [size]();
		indices = new SinglyLinkedList<size_t>();
	}


	size_t hash(const std::string& key) const {

		size_t multiplier = 1;
		size_t hashValue = 0;

		for (short c : key) {
			hashValue += multiplier * c;
			multiplier++;
		}
		return hashValue % size;
	}


	void put(const std::string& key, T &value) {


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


	T* get(const std::string& key) const {

		size_t hashValue = hash(key);
		SinglyLinkedList<HashItem<T>>* lst = slots[hashValue];

		if (!lst) {
			return nullptr;
		}

		Node<HashItem<T>>* ptr = lst->head;
		while (ptr) {
			if (ptr->data->key == key) {
				return ptr->data->value;
			}
			ptr = ptr->next;
		}
		return nullptr;
	}


	void remove(const std::string& key) {

		size_t hashValue = hash(key);
		SinglyLinkedList<HashItem<T>> *lst = slots[hashValue];

		// removing hashValue by value, and not by reference, thus cannot use indices->remove(hashValue). 
		if (*(indices->head->data) == hashValue) {
			Node<size_t> *temp = indices->head;
			indices->head = indices->head->next;
			delete(temp);
		}
		else {
			Node<size_t> *slow_i = indices->head, *fast_i = indices->head->next;
			while (fast_i) {
				if (*(fast_i->data) == hashValue) {
					slow_i->next = fast_i->next;
					fast_i = fast_i->next;
					if (*(indices->tail->data) == hashValue) {
						indices->tail = slow_i;
					}
					break;
				}
				slow_i = fast_i;
				fast_i = fast_i->next;
			}
		}


		Node<HashItem<T>> *slow = lst->head, *fast = lst->head->next;
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
			slow = fast;
			fast = fast->next;
			return;
		}
	}


	SinglyLinkedList<String>* keys() const {

		SinglyLinkedList<String>* lst = new SinglyLinkedList<String>();

		Node<size_t>* indexPtr = indices->head;

		size_t index = 0;
		while (indexPtr) {
			Node<HashItem<T>>* listPtr = slots[*(indexPtr->data)]->head;
			while (listPtr) {
				lst->append(*(new String(listPtr->data->key)));
				listPtr = listPtr->next;
				index++;
			}
			indexPtr = indexPtr->next;
		}

		return lst;
	}

	size_t get_count() const {
		return count;
	}


	std::string to_string(bool formatted=true) const {

		std::stringstream ss;
		ss << "{ ---Hash Table--- \n";
		Node<size_t> *indexPtr = indices->head;
		while (indexPtr) {

			SinglyLinkedList<HashItem<T>>* listPtr = slots[*(indexPtr->data)];
			ss << listPtr->to_string(formatted) << "\n";
		
			indexPtr = indexPtr->next;
		}
		ss << "count: " << count << ", size: " << size << "}\n";
		return ss.str();
	}
};