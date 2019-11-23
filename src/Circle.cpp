#include "Circle.h"
#include <SFML/Graphics.hpp>

Circle::Circle(float rad, float posx, float posy, float mass, sf::Color color) {
	m_radius = rad;
	m_mass = mass;

	m_velocity = sf::Vector2f(0, 0);

	c.setRadius(m_radius);
	c.setPosition(sf::Vector2f(posx, posy));
	c.setFillColor(color);
}

void Circle::setRadius(float value) {
	m_radius = value;
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

void Circle::move() {
	c.move(m_velocity);
}