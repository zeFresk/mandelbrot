#pragma once
/*
  React to sf::Event
  zeFresk
*/

namespace sf {
	class Event;
};
struct Parameters;

class EventHandler {
public:
	EventHandler();
	EventHandler(EventHandler const&) = delete;
	EventHandler& operator=(EventHandler const&) = delete;
	EventHandler(EventHandler&&) = delete;
	EventHandler& operator=(EventHandler&&) = delete;

	// react accordingly to an event in general
	void parse_event(sf::Event const& ev, Parameters& p);

private:

	// react to a keyboard event
	void parse_kb_event(sf::Event const& ev, Parameters& p);

	// react to a mouse event
	void parse_mo_event(sf::Event const& ev, Parameters& p);

	sf::Vector2f last_pos;
	bool dragging;
};