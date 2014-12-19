#include "ResourceManager.h"
#include <fstream>
#include <sstream>

ResourceManager::ResourceManager(const char* texture_file_name, const char* animations_file_name)
{
    
    sf::Image null_image;
    null_image.create(64, 64, sf::Color(0xFF, 0x00, 0xFF));
    textures["NULL_TEXTURE"] = Texture();
    textures["NULL_TEXTURE"].texture.loadFromImage(null_image);

    sf::IntRect* null_animation = new sf::IntRect[1];
    null_animation[0] = sf::IntRect(0, 0, 64, 64);
    collectionOfAnimations["NULL_ANIMATIONS"] = Animations();
    collectionOfAnimations["NULL_ANIMATIONS"].width = 64;
    collectionOfAnimations["NULL_ANIMATIONS"].height = 64;
    collectionOfAnimations["NULL_ANIMATIONS"].dimensioned = true;
    collectionOfAnimations["NULL_ANIMATIONS"].animations["null_animation"] = null_animation; 
    
    std::stringstream format;
    std::string line;
    std::string name;
    std::string file_path;
    std::string parameter;

    char direction;
    int width;
    int height;
    int x;
    int y;
    int frames;

    file_path = "res/";
    file_path += texture_file_name;

    std::ifstream in(file_path.c_str());    
    
    if (!in)
    {        
        valid = false;         
        return;   
    }
    
    while (std::getline(in, line))
    {
        format.str(line);
        format >> name;
        format >> file_path;                     
        textures[name] = Texture(file_path);   
        format.clear();
    }

    in.close();

    file_path = "res/";
    file_path += animations_file_name;

    in.open(file_path.c_str());

    if (!in)
    {
        valid = false;
        return;
    }

    while (std::getline(in, line))
    {
        format.str(line);
        format >> name;
       
        if (collectionOfAnimations.find(name) == collectionOfAnimations.end())
        {
            collectionOfAnimations[name] = Animations();
        }
        
        format >> parameter; 
        
        if (parameter == "dimensions")
        {
            format >> width;
            format >> height;
            collectionOfAnimations[name].setDimensions(width, height);
        }
        else
        {
            format >> x;
            format >> y;
            format >> direction;
            format >> frames;
            if (collectionOfAnimations[name].dimensioned)
            {
                collectionOfAnimations[name].addAnimation(parameter, x, y, direction, frames);
            }
        }
        format.clear();
    }

    valid = true;
}

ResourceManager::operator bool()
{
    return valid;
}

ResourceManager::Texture::Texture()
{
    file_name = "res/null_file";
}

ResourceManager::Texture::Texture(const std::string& file_name)
{
    this->file_name = "res/" + file_name;   
}

bool ResourceManager::Texture::load()
{
    return (loaded = texture.loadFromFile(file_name));
}

const sf::Texture& ResourceManager::getTexture(const std::string& name)
{
    if (!textures[name].loaded)
    {
        if (!textures[name].load())
        { 
            return textures["NULL_TEXTURE"].texture;
        }
    }
    return textures[name].texture;
}

ResourceManager::Animations::Animations()
{
}

void ResourceManager::Animations::setDimensions(int width, int height)
{
    this->width = width;
    this->height = height;   
    dimensioned = true;
}

void ResourceManager::Animations::addAnimation(const std::string& name, int x, int y, char direction, int frames)
{
    animations[name] = new sf::IntRect[frames];
    for (int i = 0; i < frames; i++){
        animations[name][i].width = width;
        animations[name][i].height = height;
        if (direction == 'h')
        {
            animations[name][i].left = x + (i * width);
            animations[name][i].top = y;
        }
        else if (direction == 'v')
        {
            animations[name][i].left = x;
            animations[name][i].top = y + (i * height);
        }
        else
        {
            animations[name][i].left = x;
            animations[name][i].top = y;
        }
    }
}

const std::map<std::string, sf::IntRect*>& ResourceManager::getAnimations(const std::string& name)
{
    if (collectionOfAnimations.find(name) != collectionOfAnimations.end())
        return collectionOfAnimations[name].animations;
    else
        return collectionOfAnimations["NULL_ANIMATIONS"].animations;
}

ResourceManager::Animations::~Animations()
{
    for (auto itr = animations.begin(); itr != animations.end(); itr++)
    {
        delete [] itr->second; 
    }
}

