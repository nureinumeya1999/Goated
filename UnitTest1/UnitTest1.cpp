#include "pch.h"
#include "CppUnitTest.h"

#include <iostream>
#include <string>
#include <vector>
#include "lists.h"
#include "hash.h"
#include "wrappers.h"
#include "stack.h"
#include "queue.h"
#include "Graphs/graph.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::string nodes[] = { "A", "B", "C", "D" ,"E", "F", "G", "H", "" };

std::string edges[] = {
	"A", "A",
	"A", "D",
	"A", "B",
	"A", "H",
	"B", "E",
	"C", "B",
	"C", "A",
	"D", "A",
	"D", "B",
	"D", "E",
	"D", "G",
	"F", "F",
	"F", "D",
	"G", "H",
	"H", "H",
	""
};


struct Object {
	int size = 9;
	std::string length;
	Object(std::string length) : length(length) {}
};

namespace UnitTest1
{
	TEST_CLASS(GraphTests)
	{
	public:

		TEST_CLASS_INITIALIZE(GraphInitialize) {

			Graph<Object> graph = Graph<Object>(nodes, edges);
		}

		TEST_METHOD()
		{
			
		}
	};
}
