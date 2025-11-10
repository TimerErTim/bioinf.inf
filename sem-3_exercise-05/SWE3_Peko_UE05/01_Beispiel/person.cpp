#include "pch.h"
#include "person.h"

#include <stdexcept>
#include <cctype>
#include <algorithm>

namespace {
	Gender normalize_gender(Gender g) {
		return g;
	}

	bool is_digits(std::string const& s) {
		return !s.empty() && std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c) != 0; });
	}
}

Person::Person(std::string firstName,
	std::string lastName,
	Gender gender,
	int age,
	std::string address,
	std::string creditCardNumber)
	: m_firstName(std::move(firstName))
	, m_lastName(std::move(lastName))
	, m_gender(normalize_gender(gender))
	, m_age(age)
	, m_address(std::move(address))
	, m_creditCardNumber(std::move(creditCardNumber))
{
	if (m_firstName.empty()) throw std::invalid_argument("first name must not be empty");
	if (m_lastName.empty()) throw std::invalid_argument("last name must not be empty");
	if (m_age < 0 || m_age > 130) throw std::invalid_argument("age out of range [0, 130]");
	if (m_address.empty()) throw std::invalid_argument("address must not be empty");
	if (!is_digits(m_creditCardNumber)) throw std::invalid_argument("credit card number must contain only digits");
	if (!isValidCreditCard(m_creditCardNumber)) throw std::invalid_argument("invalid credit card number (Luhn check failed)");
}

std::string Person::maskedCreditCard() const {
	if (m_creditCardNumber.size() <= 4) {
		return std::string(m_creditCardNumber.size(), '*');
	}
	std::string last4 = m_creditCardNumber.substr(m_creditCardNumber.size() - 4);
	return std::string(m_creditCardNumber.size() - 4, '*') + last4;
}

bool Person::isValidCreditCard(std::string const& digitsOnly) {
	// Luhn algorithm
	if (digitsOnly.empty()) return false;
	int sum = 0;
	bool doubleDigit = false;
	for (auto it = digitsOnly.rbegin(); it != digitsOnly.rend(); ++it) {
		int d = *it - '0';
		if (doubleDigit) {
			d *= 2;
			if (d > 9) d -= 9;
		}
		sum += d;
		doubleDigit = !doubleDigit;
	}
	return sum % 10 == 0;
}

std::ostream& operator<<(std::ostream& os, Person const& p) {
	const char* genderStr = "Diverse";
	switch (p.getGender()) {
	case Gender::Male: genderStr = "Male"; break;
	case Gender::Female: genderStr = "Female"; break;
	case Gender::Diverse: genderStr = "Diverse"; break;
	}
	os << "PERSON[" << p.getFirstName() << " " << p.getLastName()
		<< ", " << genderStr
		<< ", " << p.getAge()
		<< ", " << p.getAddress()
		<< ", " << p.maskedCreditCard() << "]";
	return os;
}


