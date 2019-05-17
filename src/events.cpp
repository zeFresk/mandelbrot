#include <SFML/Window.hpp>

#include "events.hpp"
#include "types.hpp"

EventHandler::EventHandler() : last_pos{ -1,-1 }, dragging{ false }, steps{ 10 } {}

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
	if (ev.key.code == sf::Keyboard::Up) { //
		p.center += complex_t{ 0., p.precision * steps };
	}
	else if (ev.key.code == sf::Keyboard::Down) {
		p.center -= complex_t{ 0., p.precision * steps };
	}
	else if (ev.key.code == sf::Keyboard::Right) {
		p.center += complex_t{ p.precision * steps, 0 };
	}
	else if (ev.key.code == sf::Keyboard::Left) {
		p.center -= complex_t{ p.precision * steps, 0 };
	}
	else if (ev.key.code == sf::Keyboard::Subtract) {
		p.precision *= 1.1;
	}
	else if (ev.key.code == sf::Keyboard::Add) {
		p.precision *= 0.9;
	}
	else if (ev.key.code == sf::Keyboard::Multiply) {
		p.iterations = static_cast<std::size_t>(p.iterations * 1.1);
	}
	else if (ev.key.code == sf::Keyboard::Divide) {
		p.iterations = static_cast<std::size_t>(p.iterations * 0.9);
	}
}

void EventHandler::parse_mo_event(sf::Event const& ev, Parameters& p) {

}
