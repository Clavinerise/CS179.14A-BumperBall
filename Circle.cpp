#include "Circle.h"
#include <SFML/Graphics.hpp>

float mag(sf::Vector2f v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

Circle::Circle(float rad, float posx, float posy, float mass, sf::Color color) {
	m_radius = rad;
	m_mass = mass;

	m_velocity = sf::Vector2f(0, 0);

	c.setRadius(m_radius);
	c.setPosition(sf::Vector2f(posx, posy));
	c.setOrigin(0, 0);
	c.setFillColor(color);
}

void Circle::setRadius(float value) {
	m_radius = value;
	c.setRadius(m_radius);
}

void Circle::setPosition(sf::Vector2f value) {
	c.setPosition(value);
}

void Circle::setPosition(float xval, float yval) {
	c.setPosition(sf::Vector2f(xval, yval));
}

void Circle::setMass(float mass) {
	m_mass = mass;
}

void Circle::setColor(sf::Color value) {
	c.setFillColor(value);
}

void Circle::setVelocity(sf::Vector2f value) {
	m_velocity = value;
}

void Circle::drawCircle(sf::RenderWindow &rw) {
	rw.draw(c);
}

void Circle::moveCircle() {
	c.move(m_velocity);
}

float Circle::getDistancewithCircle(Circle c) {
	sf::Vector2f c1, c2;

	c1 = getPosition();
	c2 = c.getPosition();

	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;

	return mag(sf::Vector2f(dx, dy));
}

bool Circle::isCollidingWithCircle(Circle c) {
	return getDistancewithCircle(c) <= (m_radius + c.getRadius());
}