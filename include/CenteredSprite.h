#ifndef CENTERED_SPRITE_H
#define CENTERED_SPRITE_H

#include <string>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class CenteredSprite : public sf::Sprite
{

public:
    CenteredSprite(const std::string& texture_name, ResourceManager& rm);    
    CenteredSprite(const std::string& texture_name, ResourceManager& rm, const sf::IntRect& rectangle);    

};

#endif
