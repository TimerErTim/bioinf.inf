#pragma once

#include <stdexcept>
#include <string>

class invalid_rational_error : public std::invalid_argument {
public:
	explicit invalid_rational_error(const std::string& message)
		: std::invalid_argument(message) {}
};

class division_by_zero_error : public std::domain_error {
public:
	explicit division_by_zero_error(const std::string& message)
		: std::domain_error(message) {}
};



