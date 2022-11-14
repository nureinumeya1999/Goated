#include "binary_heap.h"

double min_priority(double first, double second) {
 if (first < second) {
	 return first;
 }
 else {
	 return second;
 }
}


BinaryHeap::BinaryHeap(const std::string& title, priorityType priority) 
	: BinaryTree(title, false) {

	this->priority = priority;
	this->heap = new SinglyLinkedList<String>;
	this->heap->append(*(new String("dummy")));

	set_type("BinaryHeap");
	
}


void BinaryHeap::initialize(size_t size) {
	BinaryTree::initialize(size);
};


void BinaryHeap::initialize(std::pair<std::string, double>(&nodes)[], std::string(&edges)[], size_t size) {

	if (this->hasInitialized) {
		std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
		return;
	}
	std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
	size_t N = size ? size : 256;
	init_graph(N);
	init_nodes(nodes);
	init_edges(edges);
	validate_graph();
	this->hasInitialized = true;
	std::cout << this->to_string() << std::endl;
}


void BinaryHeap::initialize(std::vector<std::pair<std::string, double>>& nodes, std::vector<std::string>& edges, size_t size) {

	if (this->hasInitialized) {
		std::cerr << this->type << " <" << this->graphId << "> has already been initialized.";
		return;
	}
	std::cout << "\nInitializing " << this->type << " <" << this->graphId << ">...";
	size_t N = size ? size : nodes.size();
	init_graph(N);
	init_nodes(nodes);
	init_edges(edges);
	validate_graph();
	this->hasInitialized = true;
	std::cout << this->to_string() << std::endl;
}


void BinaryHeap::float_index(size_t index) {
	size_t k = index;
	while (k * 2 < count) {
		size_t m;
		String minChild;
		if (k * 2 + 1 < count) {
			minChild = priority_arg((*this->heap)[k * 2], (*this->heap)[k * 2 + 1]);
			if (minChild == (*this->heap)[k * 2]) { m = k * 2; }
			else { m = k * 2 + 1; }
		}
		else {
			minChild = (*this->heap)[k * 2];
			m = k * 2;
		}

		if ((*this->heap)[m] == priority_arg((*this->heap)[k], (*this->heap)[m])) {
			swap_nodes((*this->heap)[k], (*this->heap)[m]);
			k = m;
		}
		else {
			break;
		}
	}
}

void BinaryHeap::sink_index(size_t index) {
	size_t k = index;
	while (k > 1) {
		if ((*this->heap)[k] == priority_arg((*this->heap)[k], (*this->heap)[floor(k / 2)])) {
			swap_nodes((*this->heap)[k], (*this->heap)[floor(k / 2)]);
			k = floor(k / 2);
		}
		else {
			break;
		}
	}
}

void BinaryHeap::insert(std::pair<std::string, double>(&nodes)[]) {
	size_t i = 0;
	while (true) {
		if (std::get<0>(nodes[i]) == "") {
			break;
		}
		insert(nodes[i]);
		i++;
	}
}

void BinaryHeap::insert(std::vector<std::pair<std::string, double>>& nodes) {

	for (auto& node : nodes) {
		if (std::get<0>(node) == "") {
			break;
		}
		insert(node);
	}
}

void BinaryHeap::insert(std::pair<std::string, double>& node) {

	insert(std::get<0>(node), std::get<1>(node));
}

void BinaryHeap::insert(const std::string& id, double data) {

	create_node(id, data, this->count + 1);

	BinaryHeapNode* insert_node = get_node(id);

	if (!this->root) {
		this->set_root(get_node(id));
	}
	else {
		BinaryHeapNode* parentNode = get_node((*this->heap)[floor(count / 2)]);
		make_edge(parentNode->id.to_string(), id);
	}
	this->heap->append(insert_node->id);
	this->sink_index(count);


}


std::pair<String, Double>* BinaryHeap::pop() {
	if (count == 0) {
		return nullptr;
	}
	if (count == 1) {
		String* tempStr = new String(*this->heap->pop());
		Double* tempData = new Double(this->get_node(*tempStr)->data);
		remove_node(tempStr->to_string());
		std::pair<String, Double>* s = \
			new std::pair<String, Double>{*tempStr, *tempData};
		return s;
	}
	swap_nodes((*this->heap)[1], (*this->heap)[count]);
	String* tempStr = new String(*this->heap->pop());
	Double* tempData = new Double(this->get_node(*tempStr)->data);
	remove_node(tempStr->to_string());
	std::pair<String, Double>* s = \
		new std::pair<String, Double>{ *tempStr, *tempData };
	this->float_index(1);
	return s;
}


void BinaryHeap::sort(SinglyLinkedList<Pair<String, Double>>* memo) {
	while (this->heap->size > 1) {
		std::pair<String, Double>* curr = this->pop();
		memo->append(*(new Pair<String, Double>{ curr->first, curr->second }));
	}
}


void BinaryHeap::update_node(const std::string& node, double data) {
}




void BinaryHeap::swap_nodes(String& id1, String& id2) {

	BinaryHeapNode* node1 = static_cast<BinaryHeapNode*>(static_cast<BinaryTreeNode*>(this->get_node(id1)));
		
	BinaryHeapNode* node2 = static_cast<BinaryHeapNode*>(static_cast<BinaryTreeNode*>(this->get_node(id2)));
	
	String tempNode1Id = node1->id;
	double tempNode1data = node1->data;
	size_t tempNode1index = node1->index;

	this->nodes->put(node1->id.to_string(), *node2);
	this->nodes->put(node2->id.to_string(), *node1);

	node1->id = node2->id;
	node1->data = node2->data;
	node1->index = node2->index;
	node2->id = tempNode1Id;
	node2->data = tempNode1data;
	node2->index = tempNode1index;
};


std::string BinaryHeap::info() const {
	std::stringstream ss;
	ss << "count: " << this->count << ", root: ";
	if (this->root) {
		ss << "{id: " << this->root->id.to_string()
			<< ", data: " << static_cast<BinarySearchTreeNode*>(this->root)->data
			<< "}";
	}
	else {
		ss << "NONE";
	}
	ss << ", Heap: " << this->heap->to_string();
	return ss.str();
}

void BinaryHeap::create_node(const std::string& id, double data, size_t index) {
	this->count++;
	String* nodeId = new String(id);
	BinaryHeapNode* newGraphNode = new BinaryHeapNode(*nodeId, data, index);
	this->ids->append(*nodeId);
	this->nodes->put(id, *newGraphNode);
}


BinaryHeapNode* BinaryHeap::get_node(const std::string& id) const {
	return static_cast<BinaryHeapNode*>(this->nodes->get(id));
}


BinaryHeapNode* BinaryHeap::get_node(String& id) const {
	return static_cast<BinaryHeapNode*>(this->nodes->get(id.to_string()));
}


void BinaryHeap::init_nodes(std::vector<std::pair<std::string, double>>& nodes) {

	for (int i = 0; i < nodes.size(); i++) {
		std::string nodeId = std::get<0>(nodes[i]);
		double data = std::get<1>(nodes[i]);
		if (!this->get_node(nodeId)) {
			this->create_node(nodeId, data, false);
			count++;
		}
	}
}


void BinaryHeap::init_nodes(std::pair<std::string, double>(&nodes)[]) {

	std::pair<std::string, double>* ptr = nodes;
	while (std::get<0>(*ptr) != "") {
		std::string nodeId = std::get<0>(*ptr);
		double data = std::get<1>(*ptr);
		if (!this->get_node(nodeId)) {
			this->create_node(nodeId, data, false);
		}
		ptr++;
	}
}


void BinaryHeap::is_valid_edge(const std::string& parent, const std::string& child){
	BinaryTree::is_valid_edge(parent, child);
	this->is_valid_heap_edge(parent, child);
}


void BinaryHeap::is_valid_heap_edge(const std::string& parent, const std::string& child){
	BinaryHeapNode* parentNode = get_node(parent);
	BinaryHeapNode* childNode = get_node(child);
	
	if (this->priority_arg(parentNode->id, childNode->id) != parentNode->id) {
		std::cerr << "Parent <{"<< parent << ", " << parentNode->data 
			<< "}> does not have higher priority than child <{" 
			<< child << ", " << childNode->data << "}>." << std::endl;
		throw std::invalid_argument("Parent does not have higher priority than child.");
	}

}


void BinaryHeap::validate_graph() {
	BinaryTree::validate_tree();
	this->validate_heap();
}

void BinaryHeap::validate_heap() {
	SinglyLinkedList<String>* memo = new SinglyLinkedList<String>;
	this->breadth_first_search(this->root->id.to_string(), memo);
	size_t i = 1;
	Node<String>* ptr = memo->head;
	while (ptr) {
		get_node(*ptr->data)->index = i;
		i++;
		ptr = ptr->next;
	}
	this->heap->extend(memo);
}

