#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <string>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"
#include "ManagedSprite.h"

class AnimatedSprite : public sf::Sprite 
{
    public:
        AnimatedSprite(ResourceManager& manager, const std::string& name, 
                       const std::string& firstAnimation, int frameTime,
                       bool isCentered);
        void changeAnimation(const std::string& name);
        void changeAnimation(const std::string& name, int frameTime);
        void tick(int deltaTime);
        void play();
        void pause();
        bool isPaused();
        void changeFrameTime(int frameTime);
		void setPosition(float x, float y);
    private:        
        std::map<std::string, std::vector<sf::IntRect>> animations;
        std::string currentAnimation;
        unsigned currentFrame = 0;
        int frameTime;   
        int elapsedTime = 0;
        bool paused = false;
		bool centered;
};

#endif
