#include "AnimatedSprite.h"
#include <sstream>
#include <fstream>

AnimatedSprite::AnimatedSprite(const std::string& name, ResourceManager& rm, const std::string& firstAnimation, int frameTime) :
currentAnimation(firstAnimation), animations(rm.getAnimations(name)), sf::Sprite(rm.getTexture(name), animations[currentAnimation][currentFrame]), frameTime(frameTime) 
{
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

void AnimatedSprite::changeFrameTime(int frameTime)
{
    this->frameTime = frameTime;
}
