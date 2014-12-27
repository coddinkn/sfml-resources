#ifndef MANAGEDSPRITE_H
#define MANAGEDSPRITE_H

#include <string>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class ManagedSprite : public sf::Sprite
{
    public:
        ManagedSprite(ResourceManager& manager, const std::string& name, bool isCentered = false);
};

#endif
