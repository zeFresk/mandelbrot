#include <iostream>
#include <SFML/Graphics.hpp>

#include "types.hpp"
#include "events.hpp"

int main() {
	sf::Window window(sf::VideoMode(1280, 720), "Mandelbrot", (sf::Style::Close | sf::Style::Titlebar));
	window.setFramerateLimit(60);

	Parameters fractal_parameters{ complex_t(0,0), 0.001};
	EventHandler handler{};

	while (window.isOpen()) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) // close window
				window.close();

			handler.parse_event(ev, fractal_parameters);
		}

	}

	return 0;
}