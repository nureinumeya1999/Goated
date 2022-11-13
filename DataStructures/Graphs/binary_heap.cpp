#include "binary_heap.h"

BinaryHeap::BinaryHeap(const std::string& title, bool weighted) 
	: BinarySearchTree(title, weighted) {

	this->heap = new SinglyLinkedList<String>;

	if (weighted) {
		set_type("[Weighted]BinaryHeap");
	}
	else {
		set_type("BinaryHeap");
	}
}


void BinaryHeap::float_index(size_t index) {

}


void BinaryHeap::sink_index(size_t index) {
}


void BinaryHeap::insert(const std::string& id, double data, double weight) {
}


void BinaryHeap::pop() {
}


void BinaryHeap::remove_node(const std::string& id) {
}


void BinaryHeap::update_node(const std::string& node, double data) {
}
