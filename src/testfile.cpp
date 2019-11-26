#include <SFML/Graphics.hpp>
#include "Circle.h"

#define FPS 60
#define WINDOW_H 900
#define WINDOW_W 900
#define TIMESTEP 1.0f / FPS
#define FORCE 500.0f * TIMESTEP

float static const elasticity = 0.75f;



int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	Circle c(20.f, 100.f, 100.f, 100.f, sf::Color::White);
	Circle a(10.f, 100.f, 200.f, 250.f, sf::Color::Red);

	sf::Vector2f acceleration = sf::Vector2f(0, 0);

	bool move = false;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::D) {
					move = true;
					acceleration.x += FORCE / c.getMass();
				}
				if (event.key.code == sf::Keyboard::A) {
					move = true;
					acceleration.x -= FORCE / c.getMass();
				}
				if (event.key.code == sf::Keyboard::S) {
					move = true;
					acceleration.y += FORCE / c.getMass();
				}
				if (event.key.code == sf::Keyboard::W) {
					move = true;
					acceleration.y -= FORCE / c.getMass();
				}
			}
			if (event.type == sf::Event::KeyReleased)
				break;
		}

		if (move) {
			c.setVelocity(c.m_velocity + acceleration);
			acceleration = sf::Vector2f(0, 0);
			move = false;
		}

		c.moveCircle();

		if (c.isCollidingWithCircle(a)) {
			c.setColor(sf::Color::Blue);
		}
		else {
			c.setColor(sf::Color::White);
		}

		window.clear();
		c.drawCircle(window);
		a.drawCircle(window);
		window.display();
	}

	return 0;
}