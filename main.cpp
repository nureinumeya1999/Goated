#include <iostream>
#include <string>
#include "DataStructures/lists.h"
#include "DataStructures/hash.h"
#include "DataStructures/wrappers.h"

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
	h.put("bye", var3);
	std::cout << h.to_string() << std::endl;
	h.remove("hey");
	std::cout << h.to_string() << std::endl;

	std::cout << h.get("wsp")->to_string() << std::endl;

	

	return 0;

}


