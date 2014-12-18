#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
    ResourceManager(const char* texture_file_name, const char* animations_file_name);
    const sf::Texture& getTexture(const std::string& name);
    const std::map<std::string, sf::IntRect*>& getAnimations(const std::string& name);
    operator bool();

private:
    bool valid;
    struct Texture
    {
        Texture();
        Texture(const std::string& file_name);
        bool load();
        std::string file_name;
        bool loaded = false;
        sf::Texture texture;   
    };
    struct Animations
    {
        Animations();
        bool dimensioned = false;
        bool addAnimation(const std::string& animation_name, int x, int y, char direction, int frames);
        bool addFrame(const std::string& animation_name, int x, int y);
        int width;
        int height;
        std::string file_name;
        std::map<std::string, sf::IntRect*> animations; 
    };
    std::map<std::string, Texture> textures;
    std::map<std::string, Animations> collectionOfAnimations;
};

#endif
