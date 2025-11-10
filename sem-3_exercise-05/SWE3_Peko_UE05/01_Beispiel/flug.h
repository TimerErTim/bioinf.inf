#pragma once

#include <string>
#include <ostream>

/**
 * Flug models a single flight leg (segment) of a journey, e.g. "Linz -> Frankfurt".
 *
 * Stored attributes:
 * - flightNumber: Airline flight designator (e.g., "LH123")
 * - airline: Airline name (e.g., "Lufthansa")
 * - origin: Departure airport/city
 * - destination: Arrival airport/city
 * - departureTime: Local departure timestamp string (free format, e.g., "2025-11-10 08:30")
 * - arrivalTime: Local arrival timestamp string (free format)
 * - durationMinutes: The scheduled flight duration in minutes
 *
 * Validation:
 * - All string fields must be non-empty
 * - origin != destination
 * - durationMinutes > 0
 *
 * Note:
 * We intentionally keep times as strings to keep the example portable and
 * independent from platform-specific timezone/locale handling in this context.
 */
class Flug {
public:
	Flug(std::string flightNumber,
		std::string airline,
		std::string origin,
		std::string destination,
		std::string departureTime,
		std::string arrivalTime,
		int durationMinutes);

	const std::string& getFlightNumber() const noexcept { return m_flightNumber; }
	const std::string& getAirline() const noexcept { return m_airline; }
	const std::string& getOrigin() const noexcept { return m_origin; }
	const std::string& getDestination() const noexcept { return m_destination; }
	const std::string& getDepartureTime() const noexcept { return m_departureTime; }
	const std::string& getArrivalTime() const noexcept { return m_arrivalTime; }
	int getDurationMinutes() const noexcept { return m_durationMinutes; }

private:
	std::string m_flightNumber;
	std::string m_airline;
	std::string m_origin;
	std::string m_destination;
	std::string m_departureTime;
	std::string m_arrivalTime;
	int m_durationMinutes;
};

/**
 * Streams a concise description of one flight leg.
 * Example: FLIGHT[LH123, Lufthansa, Linz -> Frankfurt, dep 08:30, arr 10:15, 105 min]
 */
std::ostream& operator<<(std::ostream& os, Flug const& f);


