#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include <SFML/Graphics.hpp>
using namespace std;
class Circle
{
private:
	float m_radius, m_mass;
	sf::Color m_color;

public:
	sf::CircleShape c, pm;
	sf::Vector2f m_velocity;
	float m_elasticity;
	string m_type;
	float duration;
	string buff;

	Circle();
	Circle(float rad, float posx, float posy, float mass, float elasticity, sf::Color color, string type);

	float getRadius() { return m_radius; }
	sf::Vector2f getPosition() { return c.getPosition(); }
	float getMass() { return m_mass; }
	float getDistancewithCircle(Circle c);

	void setRadius(float value);
	void setPosition(sf::Vector2f value);
	void setPosition(float xval, float yval);
	void setMass(float mass);
	void setColor(sf::Color value);
	void setVelocity(sf::Vector2f value);

	void drawCircle(sf::RenderWindow& rw);
	void drawCPosMarker(sf::RenderWindow& rw);
	void moveCircle();
	bool isCollidingWithCircle(Circle c);
	void setElasticity(float set);
	void addDuration(float toAdd);
	void setTexture();
	float getElasticity();
};

#endif