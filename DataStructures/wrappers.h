#pragma once
#include <string>


class Int {
public:
	int value;

public:
	Int(int value) : value(value) {}
	int to_string() {
		return value;
	}
};

