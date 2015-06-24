#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "ManagedSprite.h"
#include "AnimatedSprite.h"

int main()
{
	sf::Clock time;

	ResourceManager manager("res/resources.txt");
	ManagedSprite track(manager, "track", "turn", true);
	sf::RenderWindow window(sf::VideoMode(448, 640), "FLOAT");
	window.setFramerateLimit(60);
	
	time.restart();

	if(!manager) return 1; 

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
		
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				track.setPosition(64 * i, 64 * j);
				if(j % 2 == 0) 
				{
					track.setRotation(90 * i);
					window.draw(track);
				}
				else
				{
					window.draw(track);
				}
			}
		}
		window.display();

	}

	return 0;
}
