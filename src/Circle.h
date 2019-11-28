#pragma once
#ifndef CIRCLE_H
#define CIRCLE_H
#include <SFML/Graphics.hpp>
class Circle
{
private:
	float m_radius, m_mass;
	sf::Color m_color;

public:
	sf::CircleShape c;
	sf::Vector2f m_velocity;

	Circle(float rad, float posx, float posy, float mass, sf::Color color);

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
	
	void drawCircle(sf::RenderWindow &rw);
	void moveCircle();
	bool isCollidingWithCircle(Circle c);
};

#endif