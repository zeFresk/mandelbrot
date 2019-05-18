#include <iostream>
#include <SFML/Graphics.hpp>

#include "types.hpp"
#include "events.hpp"
#include "mandelbrot.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Mandelbrot", (sf::Style::Close | sf::Style::Titlebar));
	window.setFramerateLimit(60);

	Parameters fractal_parameters{ complex_t(static_cast<real_t>(0),static_cast<real_t>(0)), static_cast<real_t>(0.005), 100};
	EventHandler handler{};

	sf::Texture texture;
	texture.create(window.getSize().x, window.getSize().y);
	sf::Sprite sprite(texture);

	Mandelbrot mandel{ fractal_parameters, 1280, 720};

	while (window.isOpen()) {
		// events
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) // close window
				window.close();

			handler.parse_event(ev, fractal_parameters);
		}
		window.clear();

		// computations
		std::cout << "Computing (c = " << fractal_parameters.center << " / p = " << fractal_parameters.precision << " / i = " << fractal_parameters.iterations << ")\n";
		mandel.compute();
		
		texture.update(mandel.raw_pixels()); // update texture with newly computed pixels

		window.draw(sprite);
	
		window.display();
	}

	return 0;
}