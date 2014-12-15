#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>

class ResourceManager
{

public:
    ResourceManager(const char* file_name);
    const sf::Texture& getTexture(std::string texture_name);

private:
    bool valid;
    struct Texture
    {
        Texture();
        Texture(const std::string& file_name);
        bool load();
        operator bool();
        std::string file_name;
        bool loaded = false;
        sf::Texture texture;   
    };
    std::map<std::string, Texture> textures;

};

#endif
