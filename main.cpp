#include <iostream>
#include <string>
#include "DataStructures/lists.h"
#include "DataStructures/hash.h"
#include "DataStructures/wrappers.h"
#include "DataStructures/stack.h"
#include "DataStructures/queue.h"

int main() {
	SinglyLinkedList<Int> list;
	
	Int var1 = 4, var2 = 3, var3 = 2, var4 = 1;
	list.append(var1);
	list.append(var2);
	list.append(var3);
	list.append(var4);
	std::cout << list.to_string() << std::endl;

	list.remove(var3);
	std::cout << list.to_string() << std::endl;

	SinglyLinkedList<int> **arr;
	arr = new SinglyLinkedList<int> *[30];
	Int var6 = 90;
	

	HashTable<Int> h = HashTable<Int>();
	Int var5 = 7;
	h.put("hi", var5);
	h.put("hey", var6);
	h.put("wsp", *(new Int(324)));
	h.put("hey", var6);
	std::cout << h.to_string() << std::endl;
	h.remove("hey");
	std::cout << h.to_string() << std::endl;

	std::cout << h.get("wsp")->to_string() << "\n" << std::endl;

	Stack<Int> stack = Stack<Int>();
	stack.push(var1);
	stack.push(var2);
	stack.push(var3);

	std::cout << stack.to_string() << std::endl;
	Int* t = stack.pop();
	std::cout << stack.to_string() << "POPPED " << t->to_string() << "\n" << std::endl;
	
	Queue<Int> newQ = Queue<Int>();
	newQ.enqueue(var1);
	newQ.enqueue(var2);
	newQ.enqueue(var3);

	std::cout << newQ.to_string() << std::endl;

	newQ.dequeue();
	newQ.dequeue();
	newQ.dequeue();
	newQ.dequeue();

	std::cout << newQ.to_string() << std::endl;
	return 0;

}


