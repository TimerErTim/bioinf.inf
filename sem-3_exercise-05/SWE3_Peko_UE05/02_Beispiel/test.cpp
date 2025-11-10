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
	// Arrange
	// (All fields valid except origin == destination)
	// Act & Assert
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

TEST(PersonTests, BoundaryAgesValidAndInvalid) {
	// Arrange
	std::string cc = "4111111111111111";

	// Act & Assert
	EXPECT_NO_THROW((Person("A", "B", Gender::Male, 0, "Addr", cc)));
	EXPECT_NO_THROW((Person("A", "B", Gender::Female, 130, "Addr", cc)));
	EXPECT_THROW((Person("A", "B", Gender::Diverse, -1, "Addr", cc)), std::invalid_argument);
	EXPECT_THROW((Person("A", "B", Gender::Diverse, 131, "Addr", cc)), std::invalid_argument);
}

TEST(PersonTests, ThrowsOnEmptyFields) {
	// Arrange
	std::string cc = "4111111111111111";

	// Act & Assert (Empty first/last/address)
	EXPECT_THROW((Person("", "B", Gender::Male, 10, "Addr", cc)), std::invalid_argument);
	EXPECT_THROW((Person("A", "", Gender::Male, 10, "Addr", cc)), std::invalid_argument);
	EXPECT_THROW((Person("A", "B", Gender::Male, 10, "", cc)), std::invalid_argument);
}

TEST(PersonTests, CreditCardDigitsOnlyRequired) {
	// Arrange
	std::string first = "A", last = "B", addr = "Addr";
	int age = 10;

	// Act & Assert (Non-digit characters should be rejected)
	EXPECT_THROW((Person(first, last, Gender::Male, age, addr, "4111 1111 1111 1111")), std::invalid_argument);
	EXPECT_THROW((Person(first, last, Gender::Male, age, addr, "4111-1111-1111-1111")), std::invalid_argument);
	EXPECT_THROW((Person(first, last, Gender::Male, age, addr, "abcd")), std::invalid_argument);
}

TEST(PersonTests, MaskedCreditCardShortAndLong) {
	// Arrange
	Person p_long("A", "B", Gender::Male, 20, "Addr", "4111111111111111");
	// Act
	std::string masked_long = p_long.maskedCreditCard();
	// Assert
	EXPECT_EQ(masked_long.size(), std::string("4111111111111111").size());
	EXPECT_EQ(masked_long, "************1111");

	// Arrange (length <= 4 → fully masked)
	Person p_len4("A", "B", Gender::Male, 20, "Addr", "4242");
	Person p_len3("A", "B", Gender::Male, 20, "Addr", "422");
	// Act
	std::string m4 = p_len4.maskedCreditCard();
	std::string m3 = p_len3.maskedCreditCard();
	// Assert
	EXPECT_EQ(m4, "****");
	EXPECT_EQ(m3, "***");
}

TEST(PersonTests, StreamsGenderVariants) {
	// Arrange
	Person pm("John", "Doe", Gender::Male, 30, "A", "4111111111111111");
	Person pf("Jane", "Doe", Gender::Female, 30, "A", "4111111111111111");
	Person pd("Alex", "Taylor", Gender::Diverse, 30, "A", "4111111111111111");

	// Act
	std::ostringstream osm, osf, osd;
	osm << pm;
	osf << pf;
	osd << pd;

	// Assert
	EXPECT_NE(osm.str().find("Male"), std::string::npos);
	EXPECT_NE(osf.str().find("Female"), std::string::npos);
	EXPECT_NE(osd.str().find("Diverse"), std::string::npos);
}

TEST(PersonTests, LuhnKnownVectors) {
	// Arrange
	std::string v1 = "79927398713";
	std::string v1_bad = "79927398714";
	std::string pan1 = "4242424242424242";
	std::string pan2 = "4012888888881881";

	// Act
	bool ok1 = Person::isValidCreditCard(v1);
	bool bad1 = Person::isValidCreditCard(v1_bad);
	bool ok_pan1 = Person::isValidCreditCard(pan1);
	bool ok_pan2 = Person::isValidCreditCard(pan2);

	// Assert
	EXPECT_TRUE(ok1);     // classic valid
	EXPECT_FALSE(bad1);   // classic invalid
	EXPECT_TRUE(ok_pan1);
	EXPECT_TRUE(ok_pan2);
}

TEST(FlugTests, ThrowsOnEmptyFields) {
	// Arrange
	std::string fn = "LH1", al = "LH", o = "Linz", d = "Frankfurt", dep = "08:00", arr = "09:00";
	int dur = 60;

	// Act & Assert
	EXPECT_THROW((Flug("", al, o, d, dep, arr, dur)), std::invalid_argument);
	EXPECT_THROW((Flug(fn, "", o, d, dep, arr, dur)), std::invalid_argument);
	EXPECT_THROW((Flug(fn, al, "", d, dep, arr, dur)), std::invalid_argument);
	EXPECT_THROW((Flug(fn, al, o, "", dep, arr, dur)), std::invalid_argument);
	EXPECT_THROW((Flug(fn, al, o, d, "", arr, dur)), std::invalid_argument);
	EXPECT_THROW((Flug(fn, al, o, d, dep, "", dur)), std::invalid_argument);
}

TEST(FlugTests, ThrowsOnNonPositiveDuration) {
	// Arrange
	std::string fn = "LH1", al = "LH", o = "Linz", d = "Frankfurt", dep = "08:00", arr = "09:00";

	// Act & Assert
	EXPECT_THROW((Flug(fn, al, o, d, dep, arr, 0)), std::invalid_argument);
	EXPECT_THROW((Flug(fn, al, o, d, dep, arr, -10)), std::invalid_argument);
}

TEST(FlugreiseTests, EmptyItineraryAggregates) {
	// Arrange
	Person p("X", "Y", Gender::Male, 25, "Addr", "4111111111111111");
	Flugreise r(p);

	// Act
	std::ostringstream os;
	os << r;
	std::string s = os.str();

	// Assert
	EXPECT_TRUE(r.isValidItinerary());
	EXPECT_EQ(r.totalFlightMinutes(), 0);
	EXPECT_EQ(r.firstDepartureTime(), "");
	EXPECT_EQ(r.lastArrivalTime(), "");
	EXPECT_NE(s.find("total_flight_time=0 min"), std::string::npos);
	// No window printed for empty itinerary
	EXPECT_EQ(s.find("window=") == std::string::npos, true);
}

TEST(FlugreiseTests, SingleLegWindow) {
	// Arrange
	Person p("X", "Y", Gender::Male, 25, "Addr", "4111111111111111");
	Flugreise r(p);

	// Act
	r.addFlight(Flug("OS1", "Austrian", "Linz", "Frankfurt", "08:00", "09:00", 60));
	std::ostringstream os;
	os << r;

	// Assert
	EXPECT_TRUE(r.isValidItinerary());
	EXPECT_EQ(r.totalFlightMinutes(), 60);
	EXPECT_EQ(r.firstDepartureTime(), "08:00");
	EXPECT_EQ(r.lastArrivalTime(), "09:00");
	EXPECT_NE(os.str().find("window=08:00 -> 09:00"), std::string::npos);
}

TEST(FlugreiseTests, LargeItineraryAggregation) {
	// Arrange
	Person p("Mass", "Test", Gender::Diverse, 33, "Addr", "4111111111111111");
	Flugreise r(p);

	// Act (Create 50 connected legs with increasing duration)
	int expected_total = 0;
	std::string prev_city = "C0";
	for (int i = 0; i < 50; ++i) {
		std::string next_city = "C" + std::to_string(i + 1);
		int dur = (i % 120) + 1; // 1..120 minutes
		expected_total += dur;
		r.addFlight(Flug("F" + std::to_string(i), "Air", prev_city, next_city, "T" + std::to_string(i), "T" + std::to_string(i + 1), dur));
		prev_city = next_city;
	}

	// Assert
	EXPECT_TRUE(r.isValidItinerary());
	EXPECT_EQ(r.totalFlightMinutes(), expected_total);
	EXPECT_EQ(r.firstDepartureTime(), "T0");
	EXPECT_EQ(r.lastArrivalTime(), "T50");
}

TEST(FlugreiseTests, CopySemanticsAndStreamingStable) {
	// Arrange
	Person p("Copy", "Check", Gender::Female, 29, "Addr", "4111111111111111");
	Flugreise r1(p);
	r1.addFlight(Flug("A1", "A", "L1", "L2", "08:00", "09:00", 60));
	r1.addFlight(Flug("A2", "A", "L2", "L3", "10:00", "11:30", 90));

	// Act
	std::ostringstream os1;
	os1 << r1;

	Flugreise r2 = r1; // copy
	std::ostringstream os2;
	os2 << r2;

	// Assert
	EXPECT_EQ(os1.str(), os2.str());
	EXPECT_TRUE(r2.isValidItinerary());
	EXPECT_EQ(r2.totalFlightMinutes(), 150);
}

TEST(FlugreiseTests, ConnectivityCheckedOnEachAdd) {
	// Arrange
	Person p("Conn", "Guard", Gender::Male, 44, "Addr", "4111111111111111");
	Flugreise r(p);

	// Act
	r.addFlight(Flug("S1", "S", "A", "B", "08:00", "09:00", 60));
	r.addFlight(Flug("S2", "S", "B", "C", "09:30", "10:20", 50)); // ok

	// Assert
	// Now break at third add
	EXPECT_THROW(r.addFlight(Flug("S3", "S", "X", "Y", "10:30", "11:00", 30)), std::invalid_argument);

	// Ensure previous flights intact
	EXPECT_EQ(r.getFlights().size(), 2u);
	EXPECT_TRUE(r.isValidItinerary());
}

