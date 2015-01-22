#ifndef MANAGEDSPRITE_H
#define MANAGEDSPRITE_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class ManagedSprite : public sf::Sprite
{
    public:
        ManagedSprite(ResourceManager& manager, const std::string& name, const std::string& firstFrame = "null_frame", bool isCentered = false);
        ManagedSprite(ResourceManager& manager, const std::string& name, bool isCentered);
        void changeFrame(const std::string& frame);
    private:
        std::map<std::string, sf::IntRect> frames;
};

#endif
