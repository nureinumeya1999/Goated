#pragma once
#include <string>
#include <sstream>

class Int {
public:
	int value;

public:
	Int(int value) : value(value) {}
	std::string to_string() {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
};

