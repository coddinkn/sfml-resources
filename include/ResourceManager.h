#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class ResourceManager
{

public:
    ResourceManager(const char* file_name);
    const sf::Texture& getTexture(std::string texture_name);
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
    std::map<std::string, Texture> textures;

};

#endif
