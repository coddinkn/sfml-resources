#ifndef CEN_ANIM_SPRITE_H
#define CEN_ANIM_SPRITE_H

#include <string>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.h"
#include "ResourceManager.h"

class CenAnimSprite : public AnimatedSprite
{

public:
    CenAnimSprite(const std::string& name, ResourceManager& rm, const std::string& firstAnimation, int frameTime); 

};

#endif
