#include "ResourceManager.h"

#include <iostream>
#include <iterator>
#include <ostream>
#include <fstream>
#include <sstream>

ResourceManager::ResourceManager(const std::string& resourceFilePath)
{
    // Initialize the keywords that will be parsed from the resource file
    initializeKeywords();

    // Create the default images to be used in response to invalid requests
    sf::Image defaultImage;
    defaultImage.create(64, 64, sf::Color(0xFF, 0x00, 0xFF));
    defaultTexture.loadFromImage(defaultImage);
    defaultAnimations["null_animation"].push_back(sf::IntRect(0, 0, 64, 64));
    defaultFrames["null_frame"] = sf::IntRect(0, 0, 64, 64);

    // Get the path for the parent directory of the resource file.
    // All resources will be located relative to this path
    size_t lastSeparator = resourceFilePath.find_last_of('/');
    if (lastSeparator == std::string::npos) 
        parentDirectoryPath = "";
    else
        parentDirectoryPath = resourceFilePath.substr(0, lastSeparator + 1);
    
    // Attempt to open the resource file
    std::ifstream file(resourceFilePath);
    if (!file)
    {
        valid = false;
        return;   
    }

    // Read in each instruction from the resource file
    std::string line;
    while (std::getline(file, line))
        if (line != "" && line != " " && line != "\n")
            instructions.push(split(line));

    // Parse and process each command
    int lineNumber = 1;
    std::string error;
    while (instructions.size() != 0)
    {
        std::vector<std::string> current = instructions.front();
        Command command = parseCommand(current[0]);

        // Ensure that the correct number of arguments are given
        if (checkArgumentNumber(command, current.size()))
        {
            switch (command)
            {
                case Command::ADD_ANIMATION:
                    break;
                case Command::ADD_FRAME:
                    break;
                case Command::COMMENT:
                    break;
                case Command::DIMENSION:
                    dimension(current[1], std::stoi(current[2]),
                              std::stoi(current[3]), error);
                    break;
                case Command::ESTABLISH:
                    establish(parseType(current[1]), current[2], error);
                    break;
                case Command::SOURCE:
                    source(current[1], error);
                    break;
                case Command::TEXTURE:
                    texture(current[1], current[2], current[3], error);
                    break;
                default:
                    error = "Unrecognized command \"" + current[0] + "\"";
                    break;
            }
        }
        else
        {
            error = "Invalid number of arguments for command \"" + current[0]
                    + "\"";
        }

        // Print out error information if an error occurred
        if (error != "")
        {
            std::cerr << "(ResourceManager) " << error << ':' << std::endl;
            std::cerr << "\t[" << lineNumber << "]:";
            for (int i = 0; i < current.size(); ++i)
                std::cerr << ' ' << current[i];
            std::cerr << std::endl;
        }

        // Clean up and reset error status
        instructions.pop();
        lineNumber++;
        error = "";
    }
}

/// Registers a new resource with the management system
void ResourceManager::establish(ResourceManager::Type type, 
                                const std::string& name, std::string& error)
{
    if (resources.find(name) != resources.end())
    {
        error = "Cannot establish \"" + name
                + "\", it has already been established";
    }
    if (type == Type::INVALID_TYPE)
    {
        error = "The specified type is invalid";
    }
    else // Non-error case
    {
        resources[name] = type;
    }
}

/// Inserts additional instructions from another file
void ResourceManager::source(const std::string& fileName, std::string& error)
{
    std::ifstream file(parentDirectoryPath + fileName);

    if (!file)
    {
        error = "Failed to open " + parentDirectoryPath + fileName;
    }
    else // Non-error case
    {
        std::string line;
        while (std::getline(file, line))
            instructions.push(split(line));    
    }

    file.close();
}

/// Updates the dimensions of the frames within a sprite sheet or animation
void ResourceManager::dimension(const std::string& name, int width, int height,
                                std::string& error)
{
    if (resources.find(name) == resources.end())
    {
        error = "The resource \"" + name + "\" does not exist";
    }
    else if (resources[name] == Type::SHEET)
    {
        setOfFrames[name].setDimensions(width, height);
    }
    else if (resources[name] == Type::ANIMATED)
    {
        setOfAnimations[name].setDimensions(width, height);
    }
    else
    {
        error = "The resource \"" + name + "\" is not of dimensionable type";
    }
}

void ResourceManager::texture(const std::string& name, const std::string& file,
                              const std::string& filtering, std::string& error)
{
    if (resources.find(name) == resources.end())
    {
        error = "The resource \"" + name + "\" does not exist";
    }
    else if (resources[name] == Type::INVALID_TYPE)
    {
        error = "The specified type is invalid";
    }
    else if (filtering != "smooth" && filtering != "rough")
    {
        error = "Unrecognized filtering flag \"" + filtering + "\"";
    }
    else // Non-error case
    {
        textures[name].setFileName(parentDirectoryPath + file);

        if (filtering == "smooth")
            textures[name].texture.setSmooth(true);
    }
}

void ResourceManager::newAnimation(const std::string& resourceName,
                                   const std::string& animationName, int x,
                                   int y, bool isVertical, int frameCount,
                                   std::string& error)
{
    // TODO implement
}

void ResourceManager::newFrame(const std::string& resourceName,
                               const std::string& frameName, int x, int y,
                               std::string& error)
{
    // TODO implement
}

/*
bool ResourceManager::add(std::string* arg_start, std::string* arg_end)
{
    if (resources.find(*arg_start) == resources.end())
    {
        //resource not yet created or improper number of arguments
        return false;
    }
    else if((*(arg_start + 1))[0] == 'a' && resources[*arg_start] == Type::ANIMATED)
    {
        if((arg_start + 6) < arg_end)
        {
            setOfAnimations[*arg_start].addAnimation(*(arg_start + 2), std::stoi(*(arg_start + 3)), std::stoi(*(arg_start + 4)), ((*(arg_start + 5))[0] == 'v'), std::stoi(*(arg_start + 6)));
            return true;
        }
        else
        {
            //invalid number of arguments
            return false;
        }
    }
    else if((*(arg_start + 1))[0] == 'f' && resources[*arg_start] == Type::SHEET)
    {
        if((arg_start + 4) < arg_end)
        {
            setOfFrames[*arg_start].addFrame(*(arg_start + 2), std::stoi(*(arg_start + 3)), std::stoi(*(arg_start + 4)));
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        //resource of imporper type to add specified parameter or not defined
        return false;
    }
}
*/

///// Parsing functions /////

/// Initializes the text keywords associated with each type and command
void ResourceManager::initializeKeywords()
{
    commandMap["add-animation"] = Command::ADD_ANIMATION;
    commandMap["add-frame"] = Command::ADD_FRAME;
    commandMap["#"] = Command::COMMENT;
    commandMap["establish"] = Command::ESTABLISH;
    commandMap["dimension"] = Command::DIMENSION;
    commandMap["source"] = Command::SOURCE;
    commandMap["texture"] = Command::TEXTURE;

    typeMap["animated"] = Type::ANIMATED;
    typeMap["sheet"] = Type::SHEET;
    typeMap["sprite"] = Type::SPRITE;

    elementMap["animation"];
    elementMap["frame"];

    argumentNumbers[Command::ADD_ANIMATION] = 8;
    argumentNumbers[Command::ADD_FRAME] = 6;
    argumentNumbers[Command::ESTABLISH] = 4;
    argumentNumbers[Command::DIMENSION] = 4;
    argumentNumbers[Command::SOURCE] = 2;
    argumentNumbers[Command::TEXTURE] = 4;
}

/// Finds the command associated with a given input string
ResourceManager::Command ResourceManager::parseCommand(std::string command)
{
    // Search for the command within the map
    auto matchedCommand = commandMap.find(command);

    // If the search fails, return INVALID. Otherwise, return what was found
    if (matchedCommand == commandMap.end())
        return Command::INVALID_COMMAND;
    else
        return matchedCommand->second;
}

/// Finds the resource type associated with a given input string
ResourceManager::Type ResourceManager::parseType(std::string type)
{
    // Search for the type within the map
    auto matchedType = typeMap.find(type);

    // If the search fails, return INVALID. Otherwise, return what was found
    if (matchedType == typeMap.end())
        return Type::INVALID_TYPE;
    else
        return matchedType->second;
}

ResourceManager::Element ResourceManager::parseElement(const std::string& element)
{
    // Search for the element within the appropriate keyword map
    auto matchedElement = elementMap.find(element);

    // If the search fails, return INVALID. Otherwise, return what was found
    if (matchedElement == elementMap.end())
        return Element::INVALID_ELEMENT;
    else
        return matchedElement->second;
}

// Verifies whether the number of arguments is correct for a given command
bool ResourceManager::checkArgumentNumber(Command command, int number)
{
    // Search for the number within the argument length map
    auto matchedNumber = argumentNumbers.find(command);

    // If the command is not found, assume the number of arguments doesn't
    // matter (return true)
    if (matchedNumber == argumentNumbers.end())
        return true;
    else
    {
        if (number == matchedNumber->second)
            return true;
        else
            return false;
    }
}

std::vector<std::string> ResourceManager::split(const std::string& line)
{
    // A stringstream will read in separate words and discard whitespace
    std::istringstream parser(line);
    
    // Fill a vector with every word between the start and end of the stream
    std::vector<std::string> words{std::istream_iterator<std::string>(parser),
                                   std::istream_iterator<std::string>()};

    return words;
}

///// Texture struct functions /////

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
        //using default texture for resource
        return defaultTexture;
    }
}

///// Animation struct functions /////

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
        //using default animations for resource
        return defaultAnimations;
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
        //using default frames for resource
        return defaultFrames;
    }
}
