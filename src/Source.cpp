#include <SFML/Graphics.hpp>
#include "Circle.h"
#include <fstream>
#include <iostream>
#include "Grid.cpp"
using namespace std;

#define FPS 60
#define WINDOW_H 900
#define WINDOW_W 900
#define TIMESTEP 0.5f / FPS
#define FORCE 500.0f * TIMESTEP

float dot(sf::Vector2f a, sf::Vector2f b) {
	return a.x * b.x + a.y * b.y;
}

float distance(sf::Vector2f a, sf::Vector2f b) {
	return pow(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)), 0.5);
}

struct TwoVelocities {
	sf::Vector2f v1, v2;
};

TwoVelocities setCircleCollisionVelocities(Circle b, Circle a) {

	TwoVelocities tv;
	sf::Vector2f collision_normal, relative_velocity;

	collision_normal = b.getPosition() - a.getPosition();
	relative_velocity = a.m_velocity - b.m_velocity;

	float impulse1 = -((1 + a.m_elasticity) * dot(relative_velocity, collision_normal)) / (dot(collision_normal, collision_normal) * (1 / a.getMass() + 1 / b.getMass()));
	float impulse2 = -((1 + b.m_elasticity) * dot(relative_velocity, collision_normal)) / (dot(collision_normal, collision_normal) * (1 / a.getMass() + 1 / b.getMass()));
	tv.v1 = a.m_velocity + (impulse1 / a.getMass() * collision_normal);
	tv.v2 = b.m_velocity - (impulse2 / b.getMass() * collision_normal);

	return tv;
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");

	fstream file;
	string inputFile = "Balls.txt";
	file.open(inputFile.c_str());

	float size1, posX1, posY1;
	float size2, posX2, posY2;
	file >> size1 >> posX1 >> posY1 >> size2 >> posX2 >> posY2;;
	Circle c(size1, posX1, posY1, 100.f, 1.f, sf::Color::White, "player");
	Circle d(size2, posX2, posY2, 100.f, 1.f, sf::Color::White, "player");

	std::vector<Circle*> balls;
	balls.push_back(&c);
	balls.push_back(&d);

	int nBalls;
	file >> nBalls;
	std::vector<Circle> evBalls;
	for (int x = 0; x < nBalls; x++) {
		float rad, posx, posy, mass, elasticity;
		file >> rad >> posx >> posy >> mass >> elasticity;
		evBalls.push_back(Circle(rad, posx, posy, mass, elasticity, sf::Color::Red, "ball"));
	}

	for (int x = 0; x < nBalls; x++) {
		balls.push_back(&evBalls[x]);
	}

	sf::Vector2f acceleration1 = sf::Vector2f(0, 0);
	sf::Vector2f acceleration2 = sf::Vector2f(0, 0);

	bool move1 = false, move2 = false;
	
	Grid grid;
	grid.createGrid("Test.txt", 1000, 1000);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::D) {
					move1 = true;
					acceleration1.x += FORCE / c.getMass();
				}
				if (event.key.code == sf::Keyboard::A) {
					move1 = true;
					acceleration1.x -= FORCE / c.getMass();
				}
				if (event.key.code == sf::Keyboard::S) {
					move1 = true;
					acceleration1.y += FORCE / c.getMass();
				}
				if (event.key.code == sf::Keyboard::W) {
					move1 = true;
					acceleration1.y -= FORCE / c.getMass();
				}
				if (event.key.code == sf::Keyboard::Right) {
					move2 = true;
					acceleration2.x += FORCE / d.getMass();
				}
				if (event.key.code == sf::Keyboard::Left) {
					move2 = true;
					acceleration2.x -= FORCE / d.getMass();
				}
				if (event.key.code == sf::Keyboard::Down) {
					move2 = true;
					acceleration2.y += FORCE / d.getMass();
				}
				if (event.key.code == sf::Keyboard::Up) {
					move2 = true;
					acceleration2.y -= FORCE / d.getMass();
				}
			}
			if (event.type == sf::Event::KeyReleased)
				break;
		}

		if (move1) {
			c.setVelocity(c.m_velocity + acceleration1);
			acceleration1 = sf::Vector2f(0, 0);
			move1 = false;
		}
		if (move2) {
			d.setVelocity(d.m_velocity + acceleration2);
			acceleration2 = sf::Vector2f(0, 0);
			move1 = false;
		}

		window.clear();
		grid.drawGrid(window);

		for (int x = 0; x < balls.size(); x++) {
			Circle &temp = *balls[x];

			for (int y = x + 1; y < balls.size(); y++) {
				Circle &temp2 = *balls[y];
				if (temp.isCollidingWithCircle(temp2)) {
					sf::Vector2f tmp = sf::Vector2f(
						(pow(temp.getRadius() * 2, 0.5) / 2 + pow(temp2.getRadius() * 2, 0.5) / 2),
						(pow(temp.getRadius() * 2, 0.5) / 2 + pow(temp2.getRadius() * 2, 0.5) / 2)
					);
					tmp -= temp.getPosition() - temp2.getPosition();
					tmp = sf::Vector2f(tmp.x / 4, tmp.y / 4);
					temp.c.move(-tmp);
					temp.pm.move(-tmp);
					temp2.c.move(tmp);
					temp2.pm.move(tmp);

					TwoVelocities tv = setCircleCollisionVelocities(temp, temp2);
					temp.setVelocity(tv.v2);
					temp2.setVelocity(tv.v1);
				}
			}

			if (grid.onPlatform(temp.c) && temp.type == "player") temp.setColor(sf::Color::White);
			else if(temp.type == "player") temp.setColor(sf::Color::Yellow);

			string wd = grid.wallDirection(temp.c);
			if (wd._Equal("top")) {
				temp.c.move(sf::Vector2f(0, -5));
				temp.pm.move(sf::Vector2f(0, -5));
				temp.setVelocity(sf::Vector2f(temp.m_velocity.x, -abs(temp.m_velocity.y) * temp.m_elasticity));
			}
			else if (wd._Equal("bottom")) {
				temp.c.move(sf::Vector2f(0, 5));
				temp.pm.move(sf::Vector2f(0, 5));
				temp.setVelocity(sf::Vector2f(temp.m_velocity.x, abs(temp.m_velocity.y) * temp.m_elasticity));
			}
			else if (wd._Equal("left")) {
				temp.c.move(sf::Vector2f(-5, 0));
				temp.pm.move(sf::Vector2f(-5, 0));
				temp.setVelocity(sf::Vector2f(-abs(temp.m_velocity.x), temp.m_velocity.y) * temp.m_elasticity);
			}
			else if (wd._Equal("right")) {
				temp.c.move(sf::Vector2f(5, 0));
				temp.pm.move(sf::Vector2f(5, 0));
				temp.setVelocity(sf::Vector2f(abs(temp.m_velocity.x), temp.m_velocity.y) * temp.m_elasticity);
			}

			temp.moveCircle();
			temp.drawCircle(window);
		}
		window.display();
	}

	return 0;
}