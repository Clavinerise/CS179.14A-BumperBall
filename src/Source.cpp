#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Circle.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include "Grid.cpp"
#include <thread>
using namespace std;

#define FPS 60
#define WINDOW_H 900
#define WINDOW_W 900
#define TIMESTEP 0.5f / FPS
#define FORCE 500.0f * TIMESTEP

float highSound = 45.f, lowSound = 15.f;

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

	float impulse1 = -((1 + a.getElasticity()) * dot(relative_velocity, collision_normal)) / (dot(collision_normal, collision_normal) * (1 / a.getMass() + 1 / b.getMass()));
	float impulse2 = -((1 + b.getElasticity()) * dot(relative_velocity, collision_normal)) / (dot(collision_normal, collision_normal) * (1 / a.getMass() + 1 / b.getMass()));
	tv.v1 = a.m_velocity + (impulse1 / a.getMass() * collision_normal);
	tv.v2 = b.m_velocity - (impulse2 / b.getMass() * collision_normal);

	return tv;
}

int main()
{
	sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");

	sf::SoundBuffer shootbuffer, bumpbuffer, wallbuffer;
	shootbuffer.loadFromFile("sfx/shoot.flac");
	bumpbuffer.loadFromFile("sfx/bump.wav");
	wallbuffer.loadFromFile("sfx/wall.wav");

	sf::Sound shoot, bump, wall;
	shoot.setBuffer(shootbuffer);

	bump.setBuffer(bumpbuffer);
	bump.setVolume(highSound);
	bump.setAttenuation(25.f);

	wall.setBuffer(wallbuffer);
	wall.setVolume(lowSound);
	wall.setAttenuation(25.f);


	fstream file;
	string inputFile = "ballconfigs/hard.txt";
	file.open(inputFile.c_str());

	float size1, posX1, posY1;
	float size2, posX2, posY2;
	file >> size1 >> posX1 >> posY1 >> size2 >> posX2 >> posY2;;
	Circle c(size1, posX1, posY1, 100.f, 0.5f, sf::Color::White, "player1");
	Circle d(size2, posX2, posY2, 100.f, 0.5f, sf::Color::Yellow, "player2");

	string buff1, buff2;
	string cd1, cd2;
	string reload1, reload2;

	sf::Font standard;
	standard.loadFromFile("AGENCYR.ttf");

	sf::Text p1, p2, c1, c2, r1, r2, win;

	p1.setFont(standard);
	p2.setFont(standard);

	p1.setFillColor(sf::Color::White);
	p2.setFillColor(sf::Color::Yellow);

	p1.setCharacterSize(30);
	p2.setCharacterSize(30);

	p1.setPosition(sf::Vector2f(55, 30));
	p2.setPosition(sf::Vector2f(600, 30));

	c1.setFont(standard);
	c2.setFont(standard);

	c1.setFillColor(sf::Color::White);
	c2.setFillColor(sf::Color::Yellow);

	c1.setCharacterSize(30);
	c2.setCharacterSize(30);

	c1.setPosition(sf::Vector2f(55, 65));
	c2.setPosition(sf::Vector2f(600, 65));

	r1.setFont(standard);
	r2.setFont(standard);

	r1.setFillColor(sf::Color::White);
	r2.setFillColor(sf::Color::Yellow);

	r1.setCharacterSize(30);
	r2.setCharacterSize(30);

	r1.setPosition(sf::Vector2f(55, 100));
	r2.setPosition(sf::Vector2f(600, 100));

	win.setCharacterSize(64);
	win.setFont(standard);
	win.setFillColor(sf::Color::White);
	win.setPosition(sf::Vector2f(360, 450));
	bool showWin = false;

	std::vector<Circle*> balls;
	balls.push_back(&c);
	balls.push_back(&d);

	int nBalls;
	file >> nBalls;
	std::vector<Circle> evBalls;
	for (int x = 0; x < nBalls; x++) {
		float rad, posx, posy, mass, elasticity;
		string type;
		file >> rad >> posx >> posy >> mass >> elasticity >> type;

		sf::Color powerup;
		if (type == "shootBall") {
			powerup = sf::Color::Red;
		}
		else if (type == "reducedElasticity") {
			powerup = sf::Color::Green;
		}
		else if (type == "speedBoost") {
			powerup = sf::Color::Cyan;
		}
		else {
			powerup = sf::Color::Magenta;
		}

		evBalls.push_back(Circle(rad, posx, posy, mass, elasticity, powerup, type));
	}

	for (int x = 0; x < nBalls; x++) {
		balls.push_back(&evBalls[x]);
	}

	sf::Vector2f acceleration1 = sf::Vector2f(0, 0);
	sf::Vector2f acceleration2 = sf::Vector2f(0, 0);

	bool move1 = false, move2 = false, shoot1 = false, shoot2 = false, firstShot1 = false, firstShot2 = false;
	float direction1X = 0, direction1Y = 0, direction2X = 0, direction2Y = 0;
	float cooldown1 = 5, cooldown2 = 5;

	Grid grid;
	grid.createGrid("maps/hard.txt", 1000, 1000);

	Circle tp1, tp2;

	while (window.isOpen())
	{
		if (c.duration > 0) buff1 = "Player 1 buff: " + c.buff;
		else buff1 = "Player 1 buff: ";

		if (d.duration > 0) buff2 = "Player 2 buff: " + d.buff;
		else buff2 = "Player 2 buff: ";

		reload1 = "Reloading (" + to_string((int)(5 - cooldown1 + 1)) + " s)";
		reload2 = "Reloading (" + to_string((int)(5 - cooldown2 + 1)) + " s)";

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed) {
				//p1 movement
				if (event.key.code == sf::Keyboard::D) {
					move1 = true;
					if (c.m_velocity.x < 0) acceleration1.x += 5 * FORCE / c.getMass();
					else acceleration1.x += FORCE / c.getMass();
					direction1X = 1;
					direction1Y = 0;
				}
				if (event.key.code == sf::Keyboard::A) {
					move1 = true;
					if (c.m_velocity.x > 0) acceleration1.x -= 5 * FORCE / c.getMass();
					else acceleration1.x -= FORCE / c.getMass();
					direction1X = -1;
					direction1Y = 0;
				}
				if (event.key.code == sf::Keyboard::S) {
					move1 = true;
					if (c.m_velocity.y < 0) acceleration1.y += 5 * FORCE / c.getMass();
					else acceleration1.y += FORCE / c.getMass();
					direction1Y = 1;
					direction1X = 0;
				}
				if (event.key.code == sf::Keyboard::W) {
					move1 = true;
					if (c.m_velocity.y > 0) acceleration1.y -= 5 * FORCE / c.getMass();
					else acceleration1.y -= FORCE / c.getMass();
					direction1Y = -1;
					direction1X = 0;
				}
				//p2 movement
				if (event.key.code == sf::Keyboard::Right) {
					move2 = true;
					if (d.m_velocity.x < 0) acceleration2.x += 5 * FORCE / d.getMass();
					else acceleration2.x += FORCE / d.getMass();
					direction2X = 1;
					direction2Y = 0;
				}
				if (event.key.code == sf::Keyboard::Left) {
					move2 = true;
					if (d.m_velocity.x > 0) acceleration2.x -= 5 * FORCE / d.getMass();
					else acceleration2.x -= FORCE / d.getMass();
					direction2X = -1;
					direction2Y = 0;
				}
				if (event.key.code == sf::Keyboard::Down) {
					move2 = true;
					if (d.m_velocity.y < 0) acceleration2.y += 5 * FORCE / d.getMass();
					else acceleration2.y += FORCE / d.getMass();
					direction2Y = 1;
					direction2X = 0;
				}
				if (event.key.code == sf::Keyboard::Up) {
					move2 = true;
					if (d.m_velocity.y > 0) acceleration2.y -= 5 * FORCE / d.getMass();
					else acceleration2.y -= FORCE / d.getMass();
					direction2Y = -1;
					direction2X = 0;
				}
				//shooting
				if (event.key.code == sf::Keyboard::Space && cooldown1 == 5) {
					shoot1 = true;
				}
				if (event.key.code == sf::Keyboard::Enter && cooldown2 == 5) {
					shoot2 = true;
				}
			}
			if (event.type == sf::Event::KeyReleased)
				break;
		}

		if (move1) {
			if (c.buff == "speedBoost" && c.duration > 0) c.setVelocity(c.m_velocity + acceleration1 + acceleration1 + acceleration1);
			else c.setVelocity(c.m_velocity + acceleration1);
			acceleration1 = sf::Vector2f(0, 0);
			move1 = false;
		}
		if (move2) {
			if (d.buff == "speedBoost" && d.duration > 0) d.setVelocity(d.m_velocity + acceleration2 + acceleration2 + acceleration2);
			else d.setVelocity(d.m_velocity + acceleration2);
			acceleration2 = sf::Vector2f(0, 0);
			move1 = false;
		}
		if (shoot1) {
			shoot.play();
			float strength = sqrt((c.m_velocity.x * c.m_velocity.x) + (c.m_velocity.y * c.m_velocity.y)) * 5;

			tp1 = Circle(25.f, c.getPosition().x + (c.getRadius() + 25) * direction1X, c.getPosition().y + (c.getRadius() + 25) * direction1Y, 50.f, 1.f, sf::Color::White, "ball");
			if (!firstShot1) {
				balls.push_back(&tp1);
				firstShot1 = true;
			}

			tp1.m_velocity = sf::Vector2f(strength * direction1X, strength * direction1Y);
			tp1.moveCircle();
			shoot1 = false;
			direction1X = 0;
			direction1Y = 0;
			cooldown1 = 0;
		}
		if (shoot2) {
			shoot.play();
			float strength = sqrt((d.m_velocity.x * d.m_velocity.x) + (d.m_velocity.y * d.m_velocity.y)) * 5;

			tp2 = Circle(25.f, d.getPosition().x + (d.getRadius() + 25) * direction2X, d.getPosition().y + (d.getRadius() + 25) * direction2Y, 50.f, 1.f, sf::Color::Yellow, "ball");
			if (!firstShot2) {
				balls.push_back(&tp2);
				firstShot2 = true;
			}

			tp2.m_velocity = sf::Vector2f(strength * direction2X, strength * direction2Y);
			tp2.moveCircle();
			shoot2 = false;
			direction2X = 0;
			direction2Y = 0;
			cooldown2 = 0;
		}

		float time = clock.restart().asSeconds();

		if (c.buff == "shootBall" && c.duration > 0) cooldown1 += time * 2;
		else cooldown1 += time;
		if (d.buff == "shootBall" && d.duration > 0) cooldown2 += time * 2;
		else cooldown2 += time;
		if (cooldown1 > 5) cooldown1 = 5;
		if (cooldown2 > 5) cooldown2 = 5;

		window.clear();
		grid.drawGrid(window);

		for (int x = 0; x < balls.size(); x++) {
			Circle& temp = *balls[x];

			if (temp.m_type == "player1" || temp.m_type == "player2") temp.addDuration(-time);
			else temp.addDuration(time);

			if (temp.m_type == "player1")
				cd1 = "Duration: " + to_string((int)(temp.duration + 1)) + " s";

			if (temp.m_type == "player2")
				cd2 = "Duration: " + to_string((int)(temp.duration + 1)) + " s";

			for (int y = x + 1; y < balls.size(); y++) {
				Circle& temp2 = *balls[y];
				if (temp.isCollidingWithCircle(temp2)) {
					if (temp.m_type == "player1" || temp.m_type == "player2") {
						if (temp2.m_type == "player1" || temp2.m_type == "player2") bump.setVolume(highSound);
						else bump.setVolume(lowSound);
						bump.play();
					}
					if ((temp.m_type == "player1" || temp.m_type == "player2") && temp2.duration == 15 && temp2.m_type != "ball") {
						temp.buff = temp2.m_type;
						temp.duration = temp2.duration;
						temp2.duration = 0;
					}
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

			if (!grid.onPlatform(temp.c) && temp.m_type == "player1") {
				win.setString("Player 2 Wins");
				showWin = true;
			}
			else if (!grid.onPlatform(temp.c) && temp.m_type == "player2") {
				win.setString("Player 1 Wins");
				showWin = true;
			}

			string wd = grid.wallDirection(temp.c);
			if (wd == "top") {
				if (temp.m_type == "player1" || temp.m_type == "player2") wall.play();
				temp.c.move(sf::Vector2f(0, -10));
				temp.pm.move(sf::Vector2f(0, -10));
				temp.setVelocity(sf::Vector2f(temp.m_velocity.x, -abs(temp.m_velocity.y) * temp.getElasticity()));
			}
			else if (wd == "bottom") {
				if (temp.m_type == "player1" || temp.m_type == "player2") wall.play();
				temp.c.move(sf::Vector2f(0, 10));
				temp.pm.move(sf::Vector2f(0, 10));
				temp.setVelocity(sf::Vector2f(temp.m_velocity.x, abs(temp.m_velocity.y) * temp.getElasticity()));
			}
			else if (wd == "left") {
				if (temp.m_type == "player1" || temp.m_type == "player2") wall.play();
				temp.c.move(sf::Vector2f(-10, 0));
				temp.pm.move(sf::Vector2f(-10, 0));
				temp.setVelocity(sf::Vector2f(-abs(temp.m_velocity.x), temp.m_velocity.y) * temp.getElasticity());
			}
			else if (wd == "right") {
				if (temp.m_type == "player1" || temp.m_type == "player2") wall.play();
				temp.c.move(sf::Vector2f(10, 0));
				temp.pm.move(sf::Vector2f(10, 0));
				temp.setVelocity(sf::Vector2f(abs(temp.m_velocity.x), temp.m_velocity.y) * temp.getElasticity());
			}

			temp.setTexture();
			temp.moveCircle();
			temp.drawCircle(window);

			p1.setString(buff1);
			p2.setString(buff2);

			c1.setString(cd1);
			c2.setString(cd2);

			r1.setString(reload1);
			r2.setString(reload2);

			window.draw(p1);
			window.draw(p2);

			if (c.duration > 0)
				window.draw(c1);
			if (d.duration > 0)
				window.draw(c2);

			if (cooldown1 < 5)
				window.draw(r1);
			if (cooldown2 < 5)
				window.draw(r2);

			if (showWin) {
				window.draw(win);
			}
			
		}
		window.display();

		if (showWin) {
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
			return 0;
		}
	}

	return 0;
}