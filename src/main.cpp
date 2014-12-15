#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

int main()
{

    sf::Sprite testSprite;
    ResourceManager rm("textures");

    testSprite.setTexture(rm.getTexture("test"));
    testSprite.setPosition(100, 100);

	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Resource Manager");
	window.setFramerateLimit(60);

	while ( window.isOpen() )
	{

		sf::Event event;

		while( window.pollEvent( event ) )
		{
			if( event.type == sf::Event::Closed )
            {
                window.close();
            }
		}

		window.clear();
        window.draw(testSprite);
        window.display();

	}

   	 return 0;
}
