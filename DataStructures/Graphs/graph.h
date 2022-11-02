#pragma once
#include <string>
#include <sstream>
#include "../nodes.h"
#include "../hash.h"
#include "../wrappers.h"


template <typename T>
struct GraphNode {

	String id;
	T* data;
	SinglyLinkedList<String>* children;
	SinglyLinkedList<String>* parents;


	GraphNode(String id, T& data) : id(id), data(&data),
		children(new SinglyLinkedList<String>), parents(new SinglyLinkedList<String>) {}

	GraphNode(String id) : id(id), data(nullptr),
		children(new SinglyLinkedList<String>), parents(new SinglyLinkedList<String>) {}


	std::string to_string(bool formatted=true) {

		std::stringstream ss;
		std::string sep = formatted ? "  " : " ~ ";
		
		ss << "GraphNode[ id: " << id.to_string() << sep 
			<< "children: " << children->to_string(formatted) << sep 
			<< "parents: " << parents->to_string() << " ]";

		return ss.str();
	}
};


template <typename T>
class Graph {

public:

	HashTable<GraphNode<T>>* nodes;
	SinglyLinkedList<String>* ids;
	size_t count;

public:

	template <size_t N, size_t M>
	Graph(std::string (& nodes)[N], HashTable<std::string[M]>& nodeDict) {

		count = 0;
		ids = new SinglyLinkedList<String>();
		this->nodes = new HashTable<GraphNode<T>>(count);

		for (std::string &node : nodes) {
			if (!get_node(node)) {
				create_node(node);
			}
		}

		std::string* keys = nodeDict.keys();
		size_t keyCount = nodeDict.get_count();

		for (size_t i = 0; i < keyCount; i++) {
			std::string &parent = keys[i];
			std::string (* children)[M] = nodeDict.get(parent);

			if (children) {
				for (std::string &child : *children) {
					if (child != "") {
						make_edge(get_node(child)->id, get_node(parent)->id);
					}
				}
			}
		}
	}

	GraphNode<T>* get_node(const char* id) { return nodes->get(id); }

	GraphNode<T>* get_node(std::string &id) {return nodes->get(id);}

	GraphNode<T>* get_node(String &id) {return nodes->get(id.to_string());}


	void create_node(std::string& id) {
		count++;
		String* nodeId = new String(id);
		GraphNode<T>* newGraphNode = new GraphNode<T>(*nodeId);
		ids->append(*nodeId);
		this->nodes->put(id, *newGraphNode);
	}


	void make_edge(const char* parent, const char* child) {
		make_edge(get_node(parent)->id, get_node(child)->id);}

	void make_edge(std::string& parent, std::string* child) {
		make_edge(get_node(parent)->id, get_node(child)->id);}

	void make_edge(String& parent, String& child) {

		GraphNode<T>* parentNode = get_node(parent);
		GraphNode<T>* childNode = get_node(child);

		String& childId = get_node(child)->id;
		String& parentId = get_node(parent)->id;

		if (!parentNode->children->contains(childId)) {
			parentNode->children->append(childId);
		}
		if (!childNode->parents->contains(parentId)) {
			childNode->parents->append(parentId);
		}
	}


	template <size_t M>
	void insert(std::string &node, std::string(&children)[M], std::string& parentId) {

		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;

		GraphNode<T>* parentNode = get_node(parentId);
		if (parentNode) {
			make_edge(parentNode->id, nodeId);
		}
		
		for (std::string &child : children) {
			if (child != "") {
				if (!get_node(child)) {
					create_node(child);
				}
				make_edge(nodeId, get_node(child)->id);
			}
		}
	}

	template <size_t M>
	void insert(std::string& node, std::string(&children)[M]) {

		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;

		for (std::string& child : children) {
			if (child != "") {
				if (!get_node(child)) {
					create_node(child);
				}
				make_edge(nodeId, get_node(child)->id);
			}
		}
	}

	void insert(std::string& node, std::string& parentId) {

		if (!get_node(node)) {
			create_node(node);
		}
		String& nodeId = get_node(node)->id;

		if (!get_node(parentId)) {
			create_node(parentId);
		}
		make_edge(nodeId, get_node(parentId)->id);
	}

	void insert(String& nodeId) {
		insert(nodeId.to_string());}

	void insert(const char* nodeId) {
		insert(*(new std::string(nodeId)));}

	void insert(std::string& nodeId) {

		if (!get_node(nodeId)) {
			create_node(nodeId);}
	}


	void remove_edge(const char* parentId, const char* childId) {
		remove_edge(get_node(parentId)->id, get_node(childId)->id);}

	void remove_edge(std::string &parentId, std::string &childId) {
		remove_edge(get_node(parentId)->id, get_node(childId)->id); }

	void remove_edge(String& parentId, String& childId) {
		GraphNode<T>* child = get_node(childId);
		GraphNode<T>* parent = get_node(parentId);

		SinglyLinkedList<String>* parentChildren = parent->children;
		parentChildren->remove(childId);

		SinglyLinkedList<String>* childParents = child->parents;
		childParents->remove(parentId);
	}


	void remove_node(const char* nodeId) {remove_node(get_node(nodeId)->id);}

	void remove_node(std::string& nodeId) {
		GraphNode<T>* node = get_node(nodeId);
		remove_node(node->id);}

	void remove_node(String& nodeId) {
		GraphNode<T>* node = get_node(nodeId);
		Node<String>* parentPtr = node->parents->head;
		while (parentPtr) {
			Node<String>* temp = parentPtr->next;
			String &parentId = *parentPtr->data;
			GraphNode<T>* parent = get_node(parentId);
			remove_edge(parentId, nodeId);
			parentPtr = temp;
		}
		Node<String>* childPtr = node->children->head;
		while (childPtr) {
			Node<String>* temp = childPtr->next;
			String &childId = *childPtr->data;
			GraphNode<T>* child = get_node(childId);
			remove_edge(nodeId, childId);
			childPtr = temp;
		}

		ids->remove(nodeId);
		nodes->remove(nodeId.to_string());
		count--;
	}


	std::string to_string(bool formatted = true) {

		std::stringstream ss;
		ss << "__Graph__{\n";

		if (!formatted) {
			ss << nodes->to_string(formatted);
		}
		else {
			Node<String>* currId = ids->head;

			while (currId) {
				ss << get_node(*currId->data)->to_string(formatted) << "\n";
				currId = currId->next;
			}
		}
		ss << "count: " << count << "}\n";
		return ss.str();
	}





};