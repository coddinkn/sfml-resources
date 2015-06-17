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
	AnimatedSprite test(manager, "test", "testAnimation", 1000, true);
	ManagedSprite hello(manager, "hello", "first", true);
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Resource Manager");
	window.setFramerateLimit(60);

	test.setPosition(0, 0);
	hello.setPosition(0, 0);

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
				test.rotate(5);
				hello.rotate(-5);
			}

		}

		test.tick(time.getElapsedTime().asMilliseconds());
		time.restart();

		window.clear();
		window.draw(test);
		window.draw(hello);
		window.display();

	}

	return 0;
}
