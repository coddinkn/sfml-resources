#ifndef MANAGEDSPRITE_H
#define MANAGEDSPRITE_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class ManagedSprite : public sf::Sprite
{
    public:
        ManagedSprite(ResourceManager& manager, const std::string& name, const std::string& firstFrame, bool isCentered);
        ManagedSprite(ResourceManager& manager, const std::string& name, bool isCentered);
        void changeFrame(const std::string& frame);
		void setPosition(float x, float y);
	private:
		bool centered;
        std::map<std::string, sf::IntRect> frames;
};

#endif
