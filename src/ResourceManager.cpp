#include "ResourceManager.h"

ResourceManager::ResourceManager(const char* file_name)
{
    
    sf::Image null_image;
    null_image.create(64, 64, sf::Color(0xFF, 0x00, 0xFF));
    textures["NULL_TEXTURE"] = Texture("null_file");
    textures["NULL_TEXTURE"].texture.loadFromImage(null_image);

    std::stringstream format;
    std::string line;
    std::string name;
    std::string file_path;

    file_path = "res/";
    file_path += file_name;

    std::ifstream in(file_path.c_str());    
    
    if (!in)
    {        
        std::cout << "Error opening " << file_name << " to populate texutres." << std::endl;
        std::cout << "Resource manager invalidated." << std::endl;               
        
        valid = false;         
        
        return;   
    }
    
    while (std::getline(in, line))
    {
        format.str(line);
        format >> name;
        format >> file_path;                     
        textures[name] = Texture(file_path);   
    }


    valid = true;
}

ResourceManager::operator bool()
{
    return valid;
}

ResourceManager::Texture::Texture()
{
    file_name = "null_file";
}

ResourceManager::Texture::Texture(const std::string& file_name)
{
    this->file_name = "res/" + file_name;   
}

bool ResourceManager::Texture::load()
{
    return (loaded = texture.loadFromFile(file_name));
}

const sf::Texture& ResourceManager::getTexture(std::string texture_name)
{
    if (!textures[texture_name].loaded)
    {
        if (!textures[texture_name].load())
        { 
            return textures["NULL_TEXTURE"].texture;
        }
    }
    return textures[texture_name].texture;
}
