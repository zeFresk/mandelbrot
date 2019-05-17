#include <SFML/Window.hpp>

#include "events.hpp"
#include "types.hpp"

EventHandler::EventHandler() : last_pos{-1,-1}, dragging{false} {}


void EventHandler::parse_event(sf::Event const& ev, Parameters& p) {
	switch (ev.type) {
		case sf::Event::KeyPressed:  // keyboard event
			parse_kb_event(ev, p);
			break;
		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
		case sf::Event::MouseWheelScrolled:
			parse_mo_event(ev, p);
			break;
		default:break;
	}
}

void EventHandler::parse_kb_event(sf::Event const& ev, Parameters& p) {
	if (ev.type == sf::Event::MouseButtonPressed) {
		
	}
}

void EventHandler::parse_mo_event(sf::Event const& ev, Parameters& p) {

}
