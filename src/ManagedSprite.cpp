#include "ManagedSprite.h"

ManagedSprite::ManagedSprite(ResourceManager& manager,
const std::string& name,
const std::string& firstFrame,
bool isCentered) : sf::Sprite(manager.getTexture(name)), frames(manager.getFrames(name))
{
    setTextureRect(frames[firstFrame]);
    if (isCentered)   
        setOrigin(float(getTextureRect().width / 2.0),
                  float(getTextureRect().height / 2.0));
}

ManagedSprite::ManagedSprite(ResourceManager& manager,
const std::string& name,
bool isCentered) : sf::Sprite(manager.getTexture(name)), frames(manager.getFrames(name))
{
    setTextureRect(frames["null_frame"]);
    if (isCentered)   
        setOrigin(float(getTextureRect().width / 2.0),
                  float(getTextureRect().height / 2.0));
}

void ManagedSprite::changeFrame(const std::string& frame)
{
    if (frames.find(frame) != frames.end())
        setTextureRect(frames[frame]);
    else
        setTextureRect(frames["null_frame"]);    
}
