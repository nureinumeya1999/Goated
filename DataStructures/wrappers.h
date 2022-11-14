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
	bool operator==(String other) {
		return this->to_string() == other.to_string();
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

template<typename S, typename T>
class Pair {
public:
	S first;
	T second;

public:
	Pair(S& first, T& second) : first(first), second(second) {}
	std::string to_string(bool formatted = true) const {
		std::stringstream ss;
		ss << "{" << first.to_string() << ", " << second.to_string() << "}";
		return ss.str();
	}


};
