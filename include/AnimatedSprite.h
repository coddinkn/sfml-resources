#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class AnimatedSprite : public sf::Sprite
{
public:
    AnimatedSprite(const std::string& name, ResourceManager& rm, const std::string& firstAnimation, int frameTime);
    void changeAnimation(const std::string& name);
    void changeAnimation(const std::string& name, int frameTime);
    void tick(int deltaTime);
    void play();
    void pause();
    bool isPaused();
    void changeFrameTime(int frameTime);
    ~AnimatedSprite();
private:        
    std::map<std::string, sf::IntRect*> animations;
    std::string currentAnimation;
    int currentFrame = 0;
    int frameTime;   
    int elapsedTime = 0;
    bool paused = false;
};

#endif
