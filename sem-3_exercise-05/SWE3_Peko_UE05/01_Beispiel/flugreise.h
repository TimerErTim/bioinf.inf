#pragma once

#include <vector>
#include <string>
#include <ostream>

#include "person.h"
#include "flug.h"

/**
 * Flugreise models a complete journey made up of one or more flight legs.
 *
 * Invariants:
 * - Contains at least one flight leg after construction via factory/helper or when used meaningfully
 * - The sequence of flights should be "connected": destination(i) == origin(i+1)
 *
 * Responsibility:
 * - Owns the traveller (Person) and the itinerary (vector<Flug>)
 * - Provides formatted output (operator<<)
 * - Provides validation helpers and aggregates such as total flight duration
 */
class Flugreise {
public:
	explicit Flugreise(Person traveller)
		: m_traveller(std::move(traveller)) {}

	/**
	 * Adds a flight leg to the itinerary. Throws std::invalid_argument if the
	 * new flight breaks connectivity (destination != next origin) when not empty.
	 */
	void addFlight(Flug const& flight);

	const Person& getTraveller() const noexcept { return m_traveller; }
	const std::vector<Flug>& getFlights() const noexcept { return m_flights; }

	/**
	 * Returns true if the itinerary is connected:
	 * For all i, flights[i].destination == flights[i+1].origin.
	 * Empty or single-leg itineraries are considered valid.
	 */
	bool isValidItinerary() const;

	/**
	 * Aggregates the scheduled flight time across all legs (sum of durationMinutes).
	 */
	int totalFlightMinutes() const;

	/**
	 * Returns the departure time of the first leg or an empty string if there are no legs.
	 */
	std::string firstDepartureTime() const;

	/**
	 * Returns the arrival time of the last leg or an empty string if there are no legs.
	 */
	std::string lastArrivalTime() const;

private:
	Person m_traveller;
	std::vector<Flug> m_flights;
};

/**
 * Streams a human-readable itinerary with traveller and legs.
 */
std::ostream& operator<<(std::ostream& os, Flugreise const& r);


