#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "SpriteSheet.h"
#include "AnimatedSprite.h"

int main()
{
	sf::Clock time;

	ResourceManager manager("res/resources.txt");
	SpriteSheet track = manager.newSheet("track", "turn", true);
	sf::RenderWindow window(sf::VideoMode(448, 640), "FLOAT");
	window.setFramerateLimit(60);
	AnimatedSprite test = manager.newAnimated("test", "first", 100, true);
	sf::Sprite test2 = manager.newSprite("track", true);

	time.restart();

	test.setPosition(33, 33);
	test2.setPosition(20, 100);

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
				track.changeFrame("straight");
				test2.rotate(5.0);
			}

		}

		test.tick(time.getElapsedTime().asMilliseconds());
		time.restart();

		window.clear();
		
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				track.setPosition((64 * i) + 32, (64 * j) + 32);
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
		window.draw(test2);
		window.draw(test);
		window.display();

	}

	return 0;
}
