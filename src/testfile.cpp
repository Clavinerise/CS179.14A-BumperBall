#include <SFML/Graphics.hpp>
#include <iostream>
#include "Circle.h"

#define FPS 60
#define WINDOW_H 900
#define WINDOW_W 900
#define TIMESTEP 1.0f / FPS
#define FORCE 1000.0f * TIMESTEP

using namespace std;

float static const elasticity = 1.f;

float dot(sf::Vector2f a, sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}

struct TwoVelocities {
	sf::Vector2f v1, v2;
};

TwoVelocities setCircleCollisionVelocities(Circle b, Circle a) {
	
	TwoVelocities tv;
	sf::Vector2f collision_normal, relative_velocity;

	collision_normal = b.getPosition() - a.getPosition();
	relative_velocity = a.m_velocity - b.m_velocity;

	float impulse = -((1 + elasticity) * dot(relative_velocity, collision_normal)) / (dot(collision_normal, collision_normal) * (1 / a.getMass() + 1 / b.getMass()));
	tv.v1 = a.m_velocity + (impulse / a.getMass() * collision_normal);
	tv.v2 = b.m_velocity - (impulse / b.getMass() * collision_normal);

	return tv;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
	Circle c(40.f, 100.f, 100.f, 100.f, sf::Color::White);
	Circle a(20.f, 100.f, 200.f, 250.f, sf::Color::Red);

	sf::Vector2f acceleration = sf::Vector2f(0, 0);

	bool move = false, collide = false;

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

		
		if (c.isCollidingWithCircle(a)) {
			collide = true;
		}
		else {
			collide = false;
		}

		if (collide) {
			TwoVelocities tv = setCircleCollisionVelocities(a, c);
			c.setVelocity(tv.v1);
			a.setVelocity(tv.v2);
		}

		cout << c.isCollidingWithCircle(a) << endl;
		c.moveCircle();
		a.moveCircle();

		window.clear();
		c.drawCircle(window);
		a.drawCircle(window);

		c.drawCPosMarker(window);
		a.drawCPosMarker(window);
		window.display();
	}

	return 0;
}