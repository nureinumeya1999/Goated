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


	std::string to_string() const {
		std::string sep = ", ";
		std::stringstream ss;
		ss << "key: " << key << sep << "value: " << value->to_string();
		return ss.str();
	}

	String get_id() {
		return String(key);
	}
};


template <typename T>
class HashTable {

public:

	size_t size;
	size_t count;

	SmartList<HashItem<T>>** slots;
	SmartList<size_t>* indices;

public:

	HashTable(const size_t N=NULL) {
		
		size = N ? N : 256;
		count = 0;
		slots = new SmartList<HashItem<T>> * [size]();
		indices = new SmartList<size_t>();
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
		
		SmartList<HashItem<T>> *lst = slots[hashValue];

		if (!lst)
		{
			count++;
			slots[hashValue] = new SmartList<HashItem<T>>();
			slots[hashValue]->append(*item);
			indices->append(*(new size_t(hashValue)));
			return;
		}
		
		DNode<HashItem<T>> *ptr = lst->head;
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
		SmartList<HashItem<T>>* lst = slots[hashValue];

		if (!lst) {
			return nullptr;
		}

		DNode<HashItem<T>>* ptr = lst->head;
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
		SmartList<HashItem<T>> *lst = slots[hashValue];

		indices->remove_val(hashValue);
		lst->remove_id(String(key));
	}


	SmartList<String>* keys() const {

		SmartList<String>* lst = new SmartList<String>();

		DNode<size_t>* indexPtr = indices->head;

		size_t index = 0;
		while (indexPtr) {
			DNode<HashItem<T>>* listPtr = slots[*(indexPtr->data)]->head;
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


	std::string to_string() const {

		std::stringstream ss;
		ss << "{ ---Hash Table--- \n";
		DNode<size_t> *indexPtr = indices->head;
		while (indexPtr) {

			SmartList<HashItem<T>>* listPtr = slots[*(indexPtr->data)];
			ss << listPtr->to_string() << "\n";
		
			indexPtr = indexPtr->next;
		}
		ss << "count: " << count << ", size: " << size << "}\n";
		return ss.str();
	}
};