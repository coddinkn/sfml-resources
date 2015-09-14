#ifndef MANAGEDSPRITE_H
#define MANAGEDSPRITE_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class SpriteSheet : public sf::Sprite
{
    public:
        SpriteSheet(ResourceManager& manager, const std::string& name, const std::string& firstFrame, bool isCentered);
        void changeFrame(const std::string& frame);
	private:
		bool centered;
        std::map<std::string, sf::IntRect> frames;
};

#endif
