#include "AnimatedSprite.h"
#include <sstream>
#include <fstream>

AnimatedSprite::AnimatedSprite(const std::string& name, ResourceManager& rm, const std::string& firstAnimation, int frameTime) :
currentAnimation(firstAnimation), animations(rm.getAnimations(name)), sf::Sprite(rm.getTexture(name)), frameTime(frameTime) 
{
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

void AnimatedSprite::changeAnimation(const std::string& name)
{
    currentAnimation = name;
    currentFrame = 0;
    setTextureRect(animations[currentAnimation][currentFrame]);
}

void AnimatedSprite::changeAnimation(const std::string& name, int frameTime)
{
    currentAnimation = name;
    changeFrameTime(frameTime);
    currentFrame = 0;
    setTextureRect(animations[currentAnimation][currentFrame]);
}

void AnimatedSprite::tick(int deltaTime)
{
    if (!paused)
    {        
        elapsedTime += deltaTime;
        if (elapsedTime >= frameTime) 
        {
            elapsedTime -=  frameTime;
            if (currentFrame < (animations[currentAnimation].size() - 1))
            {
                currentFrame++;
            }
            else
            {
                currentFrame = 0;
            }
            setTextureRect(animations[currentAnimation][currentFrame]);
        }
    }
}

void AnimatedSprite::changeFrameTime(int frameTime)
{
    this->frameTime = frameTime;
}
