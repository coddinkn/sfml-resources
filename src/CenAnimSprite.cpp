#include "CenAnimSprite.h"

CenAnimSprite::CenAnimSprite(const std::string&name, ResourceManager& rm, const std::string& firstAnimation, int frameTime) :
AnimatedSprite(name, rm, firstAnimation, frameTime)
{
    setOrigin(float(getTextureRect().width / 2.0), float(getTextureRect().height / 2.0));
}
