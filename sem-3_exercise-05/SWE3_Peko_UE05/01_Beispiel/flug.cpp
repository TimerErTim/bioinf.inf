#include "pch.h"
#include "flug.h"

#include <stdexcept>

Flug::Flug(std::string flightNumber,
	std::string airline,
	std::string origin,
	std::string destination,
	std::string departureTime,
	std::string arrivalTime,
	int durationMinutes)
	: m_flightNumber(std::move(flightNumber))
	, m_airline(std::move(airline))
	, m_origin(std::move(origin))
	, m_destination(std::move(destination))
	, m_departureTime(std::move(departureTime))
	, m_arrivalTime(std::move(arrivalTime))
	, m_durationMinutes(durationMinutes)
{
	if (m_flightNumber.empty()) throw std::invalid_argument("flight number must not be empty");
	if (m_airline.empty()) throw std::invalid_argument("airline must not be empty");
	if (m_origin.empty()) throw std::invalid_argument("origin must not be empty");
	if (m_destination.empty()) throw std::invalid_argument("destination must not be empty");
	if (m_origin == m_destination) throw std::invalid_argument("origin and destination must differ");
	if (m_departureTime.empty()) throw std::invalid_argument("departure time must not be empty");
	if (m_arrivalTime.empty()) throw std::invalid_argument("arrival time must not be empty");
	if (m_durationMinutes <= 0) throw std::invalid_argument("durationMinutes must be positive");
}

std::ostream& operator<<(std::ostream& os, Flug const& f) {
	os << "FLIGHT[" << f.getFlightNumber()
		<< ", " << f.getAirline()
		<< ", " << f.getOrigin() << " -> " << f.getDestination()
		<< ", dep " << f.getDepartureTime()
		<< ", arr " << f.getArrivalTime()
		<< ", " << f.getDurationMinutes() << " min]";
	return os;
}


