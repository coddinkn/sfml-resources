#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "AnimatedSprite.h"

int main()
{
    sf::Clock time;

    ResourceManager manager("res/resources.txt");
    AnimatedSprite test(manager, "test", "testAnimation", 1000);
	ManagedSprite track(manager, "track");
//   ManagedSprite test2(manager, "test2", true);
//   test2.setPosition(64, 64);
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Resource Manager");
	window.setFramerateLimit(60);

	track.setPosition(100, 100);

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
//                test.rotate(5);
            }
        
        }

//        test.tick(time.getElapsedTime().asMilliseconds());
//        time.restart();

		window.clear();
        window.draw(test);
		window.draw(track);
//        window.draw(test2);
        window.display();

	}

   	 return 0;
}
