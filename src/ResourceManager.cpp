#include "ResourceManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

ResourceManager::ResourceManager(const std::string& resourceFilePath)
{
    // Default Texture to be used when non-existant ones are requested
    sf::Image defaultImage;
    defaultImage.create(64, 64, sf::Color(0xFF, 0x00, 0xFF));
    defaultTexture.texture.loadFromImage(defaultImage);
    defaultTexture.loaded = true;
 
    // Default Animations to be used when non-existant ones are requested
    defaultAnimations.width = 64;
    defaultAnimations.height = 64;
    defaultAnimations.dimensioned = true;
    defaultAnimations.animations["null_animation"].push_back(sf::IntRect(0, 0, 64, 64));

    // Default Frames to be used when non-existant ones are requested
    defaultFrames.width = 64;
    defaultFrames.height = 64;
    defaultFrames.dimensioned = true;
    defaultFrames.frames["null_frame"] = sf::IntRect(0, 0, 64, 64);

    size_t lastSeparator = resourceFilePath.find_last_of('/');
    if (lastSeparator == std::string::npos)
        parentDirectory = "";
    else
        parentDirectory = resourceFilePath.substr(0, lastSeparator + 1);
 
    std::string line;
    std::stringstream lineStream;
    std::string argument;
    std::vector<std::string> arguments;
    std::vector<std::string> sourcedFiles;
    std::ifstream in;    
    sourcedFiles.push_back(resourceFilePath);    
        
    for (auto itr = sourcedFiles.begin(); itr != sourcedFiles.end(); ++itr)
    {
        in.open(*itr);
        if (!in)
        {        
            errorMessages.push_back(("error opening file " + *itr));
            valid = false;         
            return;   
        }
        for (int lineNumber = 1; std::getline(in, line); ++lineNumber)
        {
            if (line != "" && line[0] != '#')    
            {
                std::cout << line << std::endl;
                lineStream.str(line);
                while (lineStream >> argument)
                {            
                    arguments.push_back(argument);
                }
                if (arguments[0] == "define")
                {
                    if (!define(arguments[1], arguments.begin() + 2, arguments.end()))
                    {
                        errorMessages.push_back(("error on line " + std::to_string(lineNumber)));
                    }
                }
                else if (arguments[0] == "source")
                {
                    if (arguments.size() == 2)
                        sourcedFiles.push_back((parentDirectory + arguments[1]));
                    else
                        errorMessages.push_back(("error on line " + std::to_string(lineNumber)));
                }
                else if (resources.find(arguments[0]) != resources.end() && arguments.size() > 2)
                {
                    if (arguments[1] == "texture" && arguments.size() >= 3)
                    {
                        if (textures.find(arguments[0]) == textures.end())
                            textures[arguments[0]].setFileName(parentDirectory + arguments[2]);
                        if(arguments.size() > 4 && arguments[3] == "smooth")
                            textures[arguments[0]].texture.setSmooth(true);
                                    
                    }
                    else if (arguments[1] == "dimensions" && resources[arguments[0]] != Type::sprite && arguments.size() == 4)
                    {
                        if (resources[arguments[0]] == Type::animated)
                        {
                            setOfAnimations[arguments[0]].setDimensions(std::stoi(arguments[2]), std::stoi(arguments[3]));    
                            std::cout << setOfAnimations[arguments[0]].width << " " << setOfAnimations[arguments[0]].height << std::endl; 
                        }
                        else if (resources[arguments[0]] == Type::sheet)
                        {
                            setOfFrames[arguments[0]].setDimensions(std::stoi(arguments[2]), std::stoi(arguments[3]));    
                        }
                    }
                    else if (arguments[1] == "frame" && resources[arguments[0]] == Type::sheet && arguments.size() == 5)
                    {
                        setOfFrames[arguments[0]].addFrame(arguments[2], std::stoi(arguments[3]), std::stoi(arguments[4]));
                    }
                    else if (arguments[1] == "animation" && resources[arguments[0]] == Type::animated)
                    {
                        setOfAnimations[arguments[0]].addAnimation(arguments[2], std::stoi(arguments[3]), std::stoi(arguments[4]), (arguments[5] == "v"), std::stoi(arguments[6]));
                    }
                    else
                    {
                    
                    }
                }
                else
                {
                    errorMessages.push_back(("error on line " + std::to_string(lineNumber)));
                }
                arguments.erase(arguments.begin(), arguments.end());
                lineStream.str("");
                lineStream.clear();
            }
        }
        in.close();
    }
    valid = true;

    for (auto itr = resources.begin(); itr != resources.end(); ++itr)
    {
        std::cout << "resource name " << itr->first << std::endl;
    }

}

bool ResourceManager::define(const std::string& name, std::vector<std::string>::iterator start, std::vector<std::string>::iterator end)
{
    std::string type;
    for (auto itr = start; itr != end; ++itr)
    {
        type += *itr;   
    }
    
    std::cout << type << std::endl;
     
    if (resources.find(name) != resources.end() || name == "define" || name == "source")
    {
        return false;
    }
    else if (type == "sprite")
    {
        resources[name] = Type::sprite;
        setOfFrames[name];
        setOfFrames[name].frames[name];
        return true;
    }
    else if (type == "spritesheet") 
    {
        resources[name] = Type::sheet;
        setOfFrames[name];
        setOfFrames[name].frames[name];
        return true;
    }
    else if (type == "animatedsprite")
    {
        resources[name] = Type::animated;
        setOfAnimations[name];
        return true;
    }
    else
    {
        return false;
    }
}

ResourceManager::operator bool()
{
    return valid;
}

void ResourceManager::reportErrors(std::ostream& out)
{
    out << "----------------------------------" << "\n";
    out << "REPORTING RESOURCE MANAGER ERRORS:" << "\n";
    out << "----------------------------------" << "\n";
    for (auto itr = errorMessages.begin(); itr != errorMessages.end(); ++itr)
    {
        out << (*itr) << "\n";
    }
    out << "------------------------------------" << "\n";
    out << "END OF RESOURCE MANAGER ERROR REPORT" << "\n";
    out << "------------------------------------" << "\n";
}

ResourceManager::Texture::Texture() {}

void ResourceManager::Texture::setFileName(const std::string& file_name)
{
    this->file_name = file_name;   
}

bool ResourceManager::Texture::load()
{
    return (loaded = texture.loadFromFile(file_name));
}

const sf::Texture& ResourceManager::getTexture(const std::string& name)
{
    if (textures[name].loaded || textures[name].load())
    {
        return textures[name].texture;
    }
    else
    {
        errorMessages.push_back(("could not find or load " + name + "'s texture, using default."));
        return defaultTexture.texture;
    }
}

ResourceManager::Animations::Animations() 
{
    std::vector<sf::IntRect> nullAnimation;
    nullAnimation.push_back(sf::IntRect(0, 0, 64, 64));
    animations["null_animation"] = nullAnimation; 
}

void ResourceManager::Animations::setDimensions(int width, int height)
{
    this->width = width;
    this->height = height;   
    dimensioned = true;
}

void ResourceManager::Animations::addAnimation(const std::string& name, int x, int y, bool isVertical, int frames)
{
    for (int i = 0; i < frames; i++)
    {
        animations[name].push_back(sf::IntRect());
        animations[name][i].width = this->width;
        animations[name][i].height = this->height;

        if (isVertical)
        {
            animations[name][i].left = x;
            animations[name][i].top = y + (i * height);
        }
        else
        {
            animations[name][i].left = x + (i * width);
            animations[name][i].top = y;
        }
    }
}

const std::map<std::string, std::vector<sf::IntRect>>& ResourceManager::getAnimations(const std::string& name)
{
    if (setOfAnimations.find(name) != setOfAnimations.end())
    {
        return setOfAnimations[name].animations;
    }
    else
    {
        errorMessages.push_back(("could not find " + name + "'s animations, using default."));
        return defaultAnimations.animations;
    }
}

ResourceManager::Frames::Frames()
{
    frames["null_frame"] = sf::IntRect(0, 0, 64, 64);    
}

void ResourceManager::Frames::setDimensions(int width, int height)
{
    this->width = width;
    this->height = height;   
    dimensioned = true;
}

void ResourceManager::Frames::addFrame(const std::string& name, int x, int y)
{
    frames[name].left = x;
    frames[name].top = y;
    frames[name].width = width;
    frames[name].height = height;
}

const std::map<std::string, sf::IntRect>& ResourceManager::getFrames(const std::string& name)
{
    if (setOfFrames.find(name) != setOfFrames.end())
    {
        return setOfFrames[name].frames; 
    }
    else
    {
        errorMessages.push_back(("could not find " + name + "'s frames, using default."));
        return defaultFrames.frames;
    }
}
