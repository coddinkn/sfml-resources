#include "CenteredSprite.h"

CenteredSprite::CenteredSprite(const std::string& texture_name,
                               ResourceManager& rm) :
    CenteredSprite(rm.getTexture(texture_name))
{

}

CenteredSprite::CenteredSprite(const sf::Texture& texture) : sf::Sprite(texture)
{
    setOrigin(float(getTextureRect().width / 2.0),
              float(getTextureRect().height / 2.0));
}
