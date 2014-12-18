#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class AnimatedSprite : public sf::Sprite
{
public:
    AnimatedSprite(const std::string& name, ResourceManager& rm);
    AnimatedSprite(const std::string& name, ResourceManager& rm, const std::string& firstAnimation);
    ~AnimatedSprite();
private:        
    std::map<std::string, sf::IntRect*> frames;   
};

#endif
