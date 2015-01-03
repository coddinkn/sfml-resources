#include "ResourceManager.h"
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
    std::string command;
    std::string argument;
    std::vector<std::string> arguments;
    std::vector<std::string> sourcedFiles;
    std::map<std::string, std::string> resources;
    
    // Load the texture list from disk

    std::ifstream in(resourceFilePath);    
    
    if (!in)
    {        
        errorMessages.push_back(("error opening file " + resourceFilePath));
        valid = false;         
        return;   
    }

    for (int lineNumber = 1; std::getline(in, line); ++lineNumber)
    {
        if (line != "" && line[0] != '#') 
        {
            lineStream.str(line);
          
            if (!lineStream) 
            {
                errorMessages.push_back(("fatal error on line " + std::to_string(lineNumber) + ":"));   
                errorMessages.push_back("\terror reading line");    
                valid = false;
                return;
            }
        
            lineStream >> command;
            while (lineStream >> argument)
            {
                arguments.push_back(argument);
            }
    
            if (command == "define" && arguments.size() == 2)
            {
                if (resources.find(arguments[0]) == resources.end() && (arguments[1] == "animated" || arguments[1] == "sprite"))
                {
                    resources[arguments[0]] = arguments[1];
                }
                else
                {
                    errorMessages.push_back(("syntax error on line " + std::to_string(lineNumber) + ":"));   
                    errorMessages.push_back(("\t" + arguments[0] + " already defined"));    
                }
            }
            else if (command == "dimension")
            {
                if (resources.find(arguments[0]) != resources.end())
                {
                    if (resources[arguments[0]] == "animated")
                    {
                        if (setOfAnimations.find(arguments[0]) == setOfAnimations.end())
                        {
                            
                            
                        }
                    }
                    else if (resources[arguments[0]] == "sprite")
                    {

                    }
                }
                else
                {
                    errorMessages.push_back(("syntax error on line " + std::to_string(lineNumber) + ":"));   
                    errorMessages.push_back(("\t" + arguments[0] + " not defined"));    
                }
            }
            else if (command == "texture")
            {


            }
            else if (command == "animation")
            {


            }
            else if (command == "frame")
            {


            }
            lineStream.clear();
            lineStream.str("");
        }
    }

    in.close();

    valid = true;
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
        animations[name][i].width = width;
        animations[name][i].height = height;

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

ResourceManager::Frames::Frames() {}
