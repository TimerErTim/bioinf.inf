#include "pch.h"
#include "flugreise.h"

#include <stdexcept>
#include <sstream>

void Flugreise::addFlight(Flug const& flight) {
	if (!m_flights.empty()) {
		if (m_flights.back().getDestination() != flight.getOrigin()) {
			throw std::invalid_argument("itinerary connectivity violation: previous destination != next origin");
		}
	}
	m_flights.push_back(flight);
}

bool Flugreise::isValidItinerary() const {
	for (size_t i = 1; i < m_flights.size(); ++i) {
		if (m_flights[i - 1].getDestination() != m_flights[i].getOrigin()) {
			return false;
		}
	}
	return true;
}

int Flugreise::totalFlightMinutes() const {
	int total = 0;
	for (auto const& f : m_flights) {
		total += f.getDurationMinutes();
	}
	return total;
}

std::string Flugreise::firstDepartureTime() const {
	if (m_flights.empty()) return {};
	return m_flights.front().getDepartureTime();
}

std::string Flugreise::lastArrivalTime() const {
	if (m_flights.empty()) return {};
	return m_flights.back().getArrivalTime();
}

std::ostream& operator<<(std::ostream& os, Flugreise const& r) {
	os << "TRIP[\n  " << r.getTraveller() << "\n";
	for (auto const& f : r.getFlights()) {
		os << "  " << f << "\n";
	}
	os << "  total_flight_time=" << r.totalFlightMinutes() << " min";
	if (!r.getFlights().empty()) {
		os << ", window=" << r.firstDepartureTime() << " -> " << r.lastArrivalTime();
	}
	os << "\n]";
	return os;
}


