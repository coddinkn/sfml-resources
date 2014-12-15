#include "ResourceManager.h"

ResourceManager::ResourceManager(const char* file_name)
{

    sf::Image null_image;
    null_image.create(1, 1, sf::Color(0xFF, 0x00, 0xFF));
    textures["NULL_TEXTURE"] = Texture("null_file");
    textures["NULL_TEXTURE"].texture.loadFromImage(null_image);

    std::ifstream in(file_name);    
    
    if (!in)
    {
        
        std::cout << "Error opening " << file_name << " to populate texutres." << std::endl;
        std::cout << "Resource manager invalidated." << std::endl;               
        
        valid = false;         
        
    }
    
    std::string line;

    while (std::getline(in, line))
    {
        
            
        
        
    }
    

}

ResourceManager::Texture::Texture()
{
}

ResourceManager::Texture::Texture(const std::string& file_name)
{
    this->file_name = file_name;   
}

ResourceManager::Texture::operator bool()
{
    return loaded;
}

bool ResourceManager::Texture::load()
{
    return (loaded = texture.loadFromFile(file_name));
}
