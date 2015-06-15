#include "ManagedSprite.h"

ManagedSprite::ManagedSprite(ResourceManager& manager,
const std::string& name,
const std::string& firstFrame,
bool isCentered) : sf::Sprite(manager.getTexture(name)), frames(manager.getFrames(name))
{
    frames["null_frame"].width = getTexture()->getSize().x;
	frames["null_frame"].height = getTexture()->getSize().y;
    setTextureRect(frames[firstFrame]);
    if (isCentered)   
        setOrigin(float(getTextureRect().width / 2.0),
                  float(getTextureRect().height / 2.0));
}

ManagedSprite::ManagedSprite(ResourceManager& manager,
const std::string& name,
bool isCentered) : sf::Sprite(manager.getTexture(name)), frames(manager.getFrames(name))
{
    frames["null_frame"].width = getTexture()->getSize().x;
	frames["null_frame"].height = getTexture()->getSize().y;
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

void ManagedSprite::position(float x, float y)
{
	setPosition(x, y);

	if(getOrigin().x != 0 && getOrigin().y != 0)
		move((getTextureRect().width / 2.0), (getTextureRect().height / 2.0));
}

sf::Vector2f ManagedSprite::position()
{
	if(getOrigin().x != 0 && getOrigin().y !=0)	
	{
		return sf::Vector2f(getPosition().x + getTextureRect().width, getPosition().y + getTextureRect().height);
	}
	else
	{
		return this->getPosition();
	}
}
