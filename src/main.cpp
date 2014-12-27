#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "AnimatedSprite.h"

int main()
{
    sf::Clock time;

    ResourceManager manager("res/textures", "res/animations");
    
    if (!manager)
    {
        std::cout << "Resource Manager: Error opening files." << std::endl;
        return -1;
    }

    AnimatedSprite test(manager, "testone", "test", 1000, true);
    
    test.setPosition(100, 100);

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

                test.rotate(45);
            }
        
        }

        test.tick(time.getElapsedTime().asMilliseconds());
        time.restart();

		window.clear();
        window.draw(test);
        window.display();

	}

   	 return 0;
}
