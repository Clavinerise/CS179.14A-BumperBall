#include <SFML/Graphics.hpp>
#include <iostream>

#define FPS 60
#define WINDOW_H 800
#define WINDOW_W 800

using namespace std;

int main() {
	sf::RenderWindow window( sf::VideoMode( WINDOW_W, WINDOW_H ), "Bumper Balls" );

	window.setFramerateLimit( FPS );

	while( window.isOpen() ) {
		sf::Event event;
		switch( event.type )
		{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if( event.key.code == sf::Keyboard::Escape ) {
					window.close();
				}
				break;
		}

		window.clear();

		window.display();
	}

	return 0;
}
