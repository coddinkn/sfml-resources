#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(ResourceManager& manager,
const std::string& name,
const std::string& firstFrame,
bool isCentered) : sf::Sprite(manager.getTexture(name)), centered(isCentered), frames(manager.getFrames(name))
{
    frames["null_frame"].width = getTexture()->getSize().x;
	frames["null_frame"].height = getTexture()->getSize().y;
    setTextureRect(frames[firstFrame]);
    if (isCentered)   
        setOrigin(float(getTextureRect().width / 2.0),
                  float(getTextureRect().height / 2.0));
}

void SpriteSheet::changeFrame(const std::string& frame)
{
    if (frames.find(frame) != frames.end())
        setTextureRect(frames[frame]);
    else
        setTextureRect(frames["null_frame"]);    
}
