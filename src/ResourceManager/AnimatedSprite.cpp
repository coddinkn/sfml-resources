#include "AnimatedSprite.h"
#include <sstream>
#include <fstream>

AnimatedSprite::AnimatedSprite(ResourceManager& manager,
                               const std::string& name,
                               const std::string& firstAnimation,
                               int frameTime, bool isCentered) :
    sf::Sprite(manager.getTexture(name)),
    animations(manager.getAnimations(name))
{
    animations["null_animation"][0].width = getTexture()->getSize().x;
    animations["null_animation"][0].height = getTexture()->getSize().y;
	changeAnimation(firstAnimation, frameTime);    
    
    if(isCentered)
        setOrigin(float(getTextureRect().width / 2.0),
        float(getTextureRect().height / 2.0));
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
    if (animations.find(name) != animations.end())
        currentAnimation = name;
    else
        currentAnimation = "null_animation";
    currentFrame = 0;
    setTextureRect(animations[currentAnimation][currentFrame]);
}

void AnimatedSprite::changeAnimation(const std::string& name, int frameTime)
{
    changeAnimation(name);
    changeFrameTime(frameTime);
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
                currentFrame++;
            else
                currentFrame = 0;
            setTextureRect(animations[currentAnimation][currentFrame]);
        }
    }
}

void AnimatedSprite::changeFrameTime(int frameTime)
{
    this->frameTime = frameTime;
}
