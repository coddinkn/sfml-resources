#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "AnimatedSprite.h"

int main()
{
    sf::Clock time;

	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Resource Manager");
	window.setFramerateLimit(60);

    time.restart();

	while ( window.isOpen() )
	{

		sf::Event event;

		while( window.pollEvent( event ) )
		{
			if( event.type == sf::Event::Closed )
            {
                window.close();
            }
		
            if(event.type == sf::Event::KeyPressed)
            {

            }
        
        }

        time.restart();

		window.clear();
        window.display();

	}

   	 return 0;
}
