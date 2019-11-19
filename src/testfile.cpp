#include <SFML/Graphics.hpp>
#include "Circle.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	Circle c(20.f, 100.f, 100.f, 5.f, sf::Color::White);
	c.setVelocity(5.f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::S)
					c.c.move(0.f, c.m_velocity);
		}

		window.clear();
		c.drawCircle(window);
		window.display();
	}

	return 0;
}