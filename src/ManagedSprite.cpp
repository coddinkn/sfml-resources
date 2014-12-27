#include "ManagedSprite.h"

ManagedSprite::ManagedSprite(ResourceManager& manager,
                               const std::string& textureName,
                               bool isCentered) :
    ManagedSprite(manager.getTexture(textureName))
{

}

ManagedSprite::ManagedSprite(const sf::Texture& texture, bool isCentered) :
    sf::Sprite(texture)
{
    if (isCentered)   
        setOrigin(float(getTextureRect().width / 2.0),
                  float(getTextureRect().height / 2.0));
}
