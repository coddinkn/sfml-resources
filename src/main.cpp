#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "CenteredSprite.h"
#include "AnimatedSprite.h"

int main()
{

    sf::Clock time;

    ResourceManager rm("res/textures", "res/animations");
    
    if (!rm)
    {
        std::cout << "Resource Manager: Error opening files." << std::endl;
        return -1;
    }

    AnimatedSprite testAnimatedSprite("testone", rm, "test", 1000);
    CenteredSprite testSprite("test", rm);

    testSprite.setPosition(100, 100);
    testSprite.setScale(10, 10);

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
		
            if(event.type == sf::Event::KeyPressed){

                testSprite.rotate(45);
            }
        
        }

        testAnimatedSprite.tick(time.getElapsedTime().asMilliseconds());
        time.restart();

		window.clear();
        window.draw(testAnimatedSprite);
        window.draw(testSprite);
        window.display();

	}

   	 return 0;
}
