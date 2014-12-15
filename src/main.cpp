#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "CenteredSprite.h"

int main()
{

    ResourceManager rm("textures");
    CenteredSprite testSprite("test", rm);

    testSprite.setPosition(100, 100);

    testSprite.setScale(10, 10);

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
		
            if(event.type == sf::Event::KeyPressed){

                testSprite.rotate(45);
            }
        
        }

		window.clear();
        window.draw(testSprite);
        window.display();

	}

   	 return 0;
}
