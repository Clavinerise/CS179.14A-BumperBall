#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
using namespace std;

struct Grid {
public:
	std::vector<std::vector<sf::RectangleShape>> tiles;
	void createGrid(string inputFile, int w, int h) {
		int width = w;
		int height = h;
		fstream file;
		file.open(inputFile.c_str());
		int rows, columns;
		file >> rows >> columns;
		float tileWidth = width / columns;
		float tileHeight = height / rows;

		for (int x = 0; x < rows; x++) {
			std::vector<sf::RectangleShape> temp;
			for (int y = 0; y < columns; y++) {
				int t;
				file >> t;
				sf::RectangleShape tmp;
				if (t == 1) {
					tmp.setFillColor(sf::Color(0, 128, 0));
				}
				else if (t == 2) {
					tmp.setFillColor(sf::Color::Blue);
				}
				else {
					tmp.setFillColor(sf::Color::Black);
				}
				tmp.setSize(sf::Vector2f(tileWidth, tileHeight));
				tmp.setPosition(sf::Vector2f(y * tileWidth, x * tileHeight));
				tmp.setOrigin(0, 0);
				temp.push_back(tmp);
			}
			Grid::tiles.push_back(temp);
		}
	}
	void drawGrid(sf::RenderWindow& rw) {
		for (int x = 0; x < tiles.size(); x++) {
			for (int y = 0; y < tiles[x].size(); y++) {
				rw.draw(tiles[x][y]);
			}
		}
	}
	bool onPlatform(sf::CircleShape c) {
		for (int x = 0; x < tiles.size(); x++) {
			for (int y = 0; y < tiles[x].size(); y++) {
				if (tiles[x][y].getFillColor() == sf::Color(0, 128, 0))
				{
					if (intersects(c, tiles[x][y]))
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	string wallDirection(sf::CircleShape c) {
		for (int x = 0; x < tiles.size(); x++) {
			for (int y = 0; y < tiles[x].size(); y++) {
				if (tiles[x][y].getFillColor() == sf::Color::Blue)
				{
					if (intersects(c, tiles[x][y]))
					{
						return wall(c, tiles[x][y]);
					}
				}
			}
		}
		return "none";
	}
	bool intersects(sf::CircleShape c, sf::RectangleShape r) {
		float cx, cy, radius, rx, ry, rw, rh;
		cx = c.getPosition().x;
		cy = c.getPosition().y;
		radius = c.getRadius();
		rx = r.getPosition().x;
		ry = r.getPosition().y;
		rw = r.getSize().x;
		rh = r.getSize().y;
		return circleRect(cx, cy, radius, rx, ry, rw, rh);
	}
	bool circleRect(float cx, float cy, float radius, float rx, float ry, float rw, float rh) {

		// temporary variables to set edges for testing
		float testX = cx;
		float testY = cy;

		// which edge is closest?
		if (cx < rx)         testX = rx;      // test left edge
		else if (cx > rx + rw) testX = rx + rw;   // right edge
		if (cy < ry)         testY = ry;      // top edge
		else if (cy > ry + rh) testY = ry + rh;   // bottom edge

		// get distance from closest edges
		float distX = cx - testX;
		float distY = cy - testY;
		float distance = sqrt((distX * distX) + (distY * distY));

		// if the distance is less than the radius, collision!
		if (distance <= radius) {
			return true;
		}
		return false;
	}
	string wall(sf::CircleShape c, sf::RectangleShape r) {
		float cx = c.getPosition().x;
		float cy = c.getPosition().y;
		float radius = c.getRadius();
		float rx = r.getPosition().x;
		float ry = r.getPosition().y;
		float rw = r.getSize().x;
		float rh = r.getSize().y;
		string check = "";
		if (cx < rx) {
			check = "left";
		}
		else if (cx > rx + rw)
		{
			check = "right";
		}
		if (cy < ry) {
			check = "top";
		}
		else if (cy > ry + rh) {
			check = "bottom";
		}
		return check;
	}
};
