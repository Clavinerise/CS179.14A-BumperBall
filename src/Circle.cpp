#include "Circle.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

float mag(sf::Vector2f v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

Circle::Circle() {

}

Circle::Circle(float rad, float posx, float posy, float mass, float elasticity, sf::Color color, string type) {
	m_radius = rad;
	m_mass = mass;
	m_velocity = sf::Vector2f(0, 0);
	m_elasticity = elasticity;
	m_type = type;

	if (type == "player1" || type == "player2" || type == "ball") duration = 0;
	else duration = 15;

	c.setRadius(m_radius);
	c.setPosition(sf::Vector2f(posx, posy));
	c.setOrigin(sf::Vector2f(m_radius, m_radius));
	c.setFillColor(color);

	pm.setRadius(2.5);
	pm.setPosition(sf::Vector2f(posx, posy));
	pm.setOrigin(sf::Vector2f(2.5, 2.5));
	pm.setFillColor(sf::Color::Blue);

	setTexture();
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
	if (value.x > 1.f) {
		value.x = 1.f;
	}
	if (value.y > 1.f) {
		value.y = 1.f;
	}
	if (value.x < -1.f) {
		value.x = -1.f;
	}
	if (value.y < -1.f) {
		value.y = -1.f;
	}
	if (buff == "speedBoost" && duration > 0) {
		value.x *= 1.2;
		value.y *= 1.2;
	}
	m_velocity = value;
}

void Circle::drawCircle(sf::RenderWindow& rw) {
	rw.draw(c);
}

void Circle::moveCircle() {
	c.move(m_velocity);
	pm.move(m_velocity);
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

void Circle::drawCPosMarker(sf::RenderWindow& rw) {
	rw.draw(pm);
}

void Circle::setElasticity(float set) {
	m_elasticity = set;
}

void Circle::addDuration(float toAdd) {
	duration += toAdd;
	if (duration > 15) duration = 15;
	if (duration < 0) duration = 0;
}

void Circle::setTexture() {
	sf::Texture texture;
	sf::Texture textureBall;
	textureBall.loadFromFile("ball.png");
	if (m_type == "player1" || m_type == "player2") {

	}
	else if (m_type == "speedBoost") {
		texture.loadFromFile("speedboost.png");
		if (duration == 15) c.setTexture(&texture);
		else c.setTexture(&textureBall);
	}
	else if (m_type == "shootBall") {
		texture.loadFromFile("shootBall.png");
		if (duration == 15) c.setTexture(&texture);
		else c.setTexture(&textureBall);
	}
	else if (m_type == "reducedElasticity") {
		texture.loadFromFile("reducedElasticity.png");
		if (duration == 15) c.setTexture(&texture);
		else c.setTexture(&textureBall);
	}
	else if (m_type == "ball") {
		c.setTexture(&textureBall);
	}
}

float Circle::getElasticity() {
	if (buff == "reducedElasticity" && duration > 0) return 0;
	return m_elasticity;
}