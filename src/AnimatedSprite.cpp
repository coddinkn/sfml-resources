#include "AnimatedSprite.h"
#include <algorithm>
#include <sstream>
#include <fstream>

AnimatedSprite::AnimatedSprite(const std::string& name, ResourceManager& rm, const std::string& firstAnimation, int frameTime) :
currentAnimation(firstAnimation), sf::Sprite(rm.getTexture(name)), frameTime(frameTime) 
{
    animations = rm.getAnimations(name);
    setTextureRect(animations[currentAnimation][currentFrame]);
}

void AnimatedSprite::play()
{
    paused = false;
}

void AnimatedSprite::pause()
{
    paused = true;
}

bool AnimatedSprite::isPaused()
{
    return paused;
}

void AnimatedSprite::tick(int deltaTime)
{


}

void AnimatedSprite::changeFrameTime(int frameTime)
{
    this->frameTime = frameTime;
}
