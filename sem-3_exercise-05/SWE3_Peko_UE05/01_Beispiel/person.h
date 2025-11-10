#pragma once

#include <string>
#include <ostream>

/**
 * Represents the legal gender of a person.
 */
enum class Gender {
	Male,
	Female,
	Diverse
};

/**
 * Person models the customer/traveller for a trip.
 *
 * Invariants:
 * - first name and last name are non-empty
 * - age is in [0, 130]
 * - address is non-empty
 * - credit card number is a string of digits that passes the Luhn check
 */
class Person {
public:
	/**
	 * Creates a new person with validated attributes.
	 * Throws std::invalid_argument on invalid inputs.
	 */
	Person(std::string firstName,
		std::string lastName,
		Gender gender,
		int age,
		std::string address,
		std::string creditCardNumber);

	const std::string& getFirstName() const noexcept { return m_firstName; }
	const std::string& getLastName() const noexcept { return m_lastName; }
	Gender getGender() const noexcept { return m_gender; }
	int getAge() const noexcept { return m_age; }
	const std::string& getAddress() const noexcept { return m_address; }
	const std::string& getCreditCardNumber() const noexcept { return m_creditCardNumber; }

	/**
	 * Masks the credit card number for display: returns "************1234" (keeps last 4).
	 * If number length < 4, masks all digits.
	 */
	std::string maskedCreditCard() const;

	/**
	 * Validates the credit card number using the Luhn algorithm.
	 * Returns true if the string consists of digits and passes Luhn.
	 */
	static bool isValidCreditCard(std::string const& digitsOnly);

private:
	std::string m_firstName;
	std::string m_lastName;
	Gender m_gender;
	int m_age;
	std::string m_address;
	std::string m_creditCardNumber; // stored as provided (digits only requirement enforced)
};

/**
 * Streams a concise single-line description of the person.
 * Example: Person[John Doe, Male, 35, 101 Main St, ****1234]
 */
std::ostream& operator<<(std::ostream& os, Person const& p);


