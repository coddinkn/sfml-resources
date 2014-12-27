#include "ManagedSprite.h"

ManagedSprite::ManagedSprite(ResourceManager& manager,
const std::string& textureName,
bool isCentered) : sf::Sprite(manager.getTexture(textureName))
{
    if (isCentered)   
        setOrigin(float(getTextureRect().width / 2.0),
                  float(getTextureRect().height / 2.0));
}
