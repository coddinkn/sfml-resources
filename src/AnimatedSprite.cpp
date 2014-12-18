#include "AnimatedSprite.h"
#include <sstream>
#include <fstream>

AnimatedSprite::AnimatedSprite(const std::string& name, ResourceManager& rm)
{
}

AnimatedSprite::AnimatedSprite(const std::string& name, ResourceManager& rm, const std::string& firstAnimation)
{
}

AnimatedSprite::~AnimatedSprite()
{
    for (auto itr = frames.begin(); itr != frames.end(); itr++)
    {
        delete [] itr->second;
    }
}
