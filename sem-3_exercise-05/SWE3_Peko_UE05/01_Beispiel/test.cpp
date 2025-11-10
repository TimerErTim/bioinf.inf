#include "pch.h"
#include "person.h"
#include "flug.h"
#include "flugreise.h"

#include <sstream>

// Tests follow the AAA (Arrange-Act-Assert) principle.

TEST(PersonTests, ConstructsWithValidData) {
	// Arrange
	std::string cc = "4111111111111111"; // Luhn valid (Visa test number)

	// Act
	Person p("John", "Doe", Gender::Male, 35, "101 Main St", cc);

	// Assert
	EXPECT_EQ(p.getFirstName(), "John");
	EXPECT_EQ(p.getLastName(), "Doe");
	EXPECT_EQ(p.getAge(), 35);
	EXPECT_EQ(p.getAddress(), "101 Main St");
	EXPECT_TRUE(Person::isValidCreditCard(cc));
	EXPECT_TRUE(p.maskedCreditCard().size() == cc.size());
	EXPECT_EQ(p.maskedCreditCard().substr(cc.size() - 4), "1111");

	std::ostringstream os;
	os << p;
	EXPECT_NE(os.str().find("****"), std::string::npos);
}

TEST(PersonTests, ThrowsOnInvalidCreditCard) {
	// Arrange
	std::string bad = "1234567890123456"; // fails Luhn

	// Act + Assert
	EXPECT_THROW((Person("A", "B", Gender::Diverse, 20, "Somewhere", bad)), std::invalid_argument);
}

TEST(FlugTests, ConstructsAndStreams) {
	// Arrange
	Flug f("LH123", "Lufthansa", "Linz", "Frankfurt", "2025-11-10 08:30", "2025-11-10 09:45", 75);

	// Act
	std::ostringstream os;
	os << f;

	// Assert
	std::string s = os.str();
	EXPECT_NE(s.find("LH123"), std::string::npos);
	EXPECT_NE(s.find("Linz -> Frankfurt"), std::string::npos);
	EXPECT_NE(s.find("75 min"), std::string::npos);
}

TEST(FlugTests, ThrowsOnSameOriginDestination) {
	EXPECT_THROW((Flug("LH1", "LH", "Linz", "Linz", "08:00", "09:00", 60)), std::invalid_argument);
}

TEST(FlugreiseTests, ValidItineraryAggregation) {
	// Arrange
	Person p("Jane", "Roe", Gender::Female, 28, "Street 1", "4111111111111111");
	Flugreise r(p);

	// Act
	r.addFlight(Flug("OS1", "Austrian", "Linz", "Frankfurt", "08:00", "09:00", 60));
	r.addFlight(Flug("UA2", "United", "Frankfurt", "Denver", "10:30", "18:00", 510));
	r.addFlight(Flug("WN3", "Southwest", "Denver", "Las Vegas", "19:00", "20:30", 90));

	// Assert
	EXPECT_TRUE(r.isValidItinerary());
	EXPECT_EQ(r.totalFlightMinutes(), 60 + 510 + 90);
	EXPECT_EQ(r.firstDepartureTime(), "08:00");
	EXPECT_EQ(r.lastArrivalTime(), "20:30");

	std::ostringstream os;
	os << r;
	std::string printed = os.str();
	EXPECT_NE(printed.find("TRIP["), std::string::npos);
	EXPECT_NE(printed.find("total_flight_time=660 min"), std::string::npos);
	EXPECT_NE(printed.find("Linz -> Frankfurt"), std::string::npos);
	EXPECT_NE(printed.find("Frankfurt -> Denver"), std::string::npos);
	EXPECT_NE(printed.find("Denver -> Las Vegas"), std::string::npos);
}

TEST(FlugreiseTests, RejectsBrokenConnectivity) {
	// Arrange
	Person p("A", "B", Gender::Diverse, 40, "X", "4111111111111111");
	Flugreise r(p);

	// Act
	r.addFlight(Flug("X1", "X", "Linz", "Frankfurt", "08:00", "09:00", 60));

	// Assert
	EXPECT_THROW(r.addFlight(Flug("X2", "X", "Munich", "Denver", "10:00", "12:00", 120)), std::invalid_argument);
}

