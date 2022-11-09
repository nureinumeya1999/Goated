#pragma once
#include <string>
#include <sstream>

class Int {
public:
	int value;

public:
	Int(int value) : value(value) {}
	std::string to_string(bool formatted=true) {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
};

class String {
public:
	std::string value = "";

public:
	String() : value("") {}
	String(const std::string& value) : value(value) {}
	std::string to_string(bool formatted = true) const {
		return value;
	}
};


class Double {
public:
	double value;

public:
	Double(const double& value) : value(value) {}
	std::string to_string(bool formatted = true) const {
		std::stringstream ss;
		ss << value;
		return ss.str();
	}
};
