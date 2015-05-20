#include "ResourceManager.h"

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
    while (instructions.size() != 0)
    {
        std::vector<std::string> current = instructions.front();

        switch (parseCommand(current[0]))
        {
            case Command::ADD:
                std::cout << "add" << std::endl;
                break;
            case Command::COMMENT:
                std::cout << "comment" << std::endl;
                break;
            case Command::DIMENSION:
                std::cout << "dimension" << std::endl;
                break;
            case Command::ESTABLISH:
                std::cout << "establish" << std::endl;
                break;
            case Command::SOURCE:
                std::cout << "source" << std::endl;
                break;
            case Command::TEXTURE:
                std::cout << "texture" << std::endl;
                break;
            default:
                std::cout << "invalid command" << std::endl;
                break;
        }
        /*
        switch (instructions.front().command)
        {
            case '#':            
				//comment
                break;
            case 'c':
                if(!establish(instructions.front().arguments + 1, instructions.front().arguments + instructions.front().arg_length)) std::cerr << instructions.front() << "\terror: improper type or redefinition of resource" << std::endl; 
                break;
            case 's':
                if(!source(instructions.front().arguments + 1, instructions.front().arguments + instructions.front().arg_length)) std::cerr << instructions.front() << "\terror: problem opening file(s) being sourced" << std::endl; 
                break;
            case 'd':
                if(!dimension(instructions.front().arguments + 1, instructions.front().arguments + instructions.front().arg_length)) std::cerr << instructions.front() << "\terror: resource not created or of non dimensionable type" << std::endl; 
				break;
            case 't':
                if(!texture(instructions.front().arguments + 1, instructions.front().arguments + instructions.front().arg_length)) std::cerr << instructions.front() << "\terror: resource not created or of non texturable type" << std::endl; 
                break;
            case 'a':
                if(!add(instructions.front().arguments + 1, instructions.front().arguments + instructions.front().arg_length)) std::cerr << instructions.front() << "\terror: resource could not add specified parameter" << std::endl; 
                break;
            default:  
                //invalid command
                break; 
        }

        delete [] instructions.front().arguments;
        */

        instructions.pop();
    }
}

bool ResourceManager::establish(std::string* arg_start, std::string* arg_end)
{
    std::string type = "";
    for(auto itr = (arg_start + 1); itr != arg_end; ++itr) type += *itr;

	if(resources.find(*arg_start) != resources.end())
	{
		//resource already created
		return false;
	}
    else if(type == "animatedsprite") 
    {
        resources[*arg_start] = Type::ANIMATED;
		return true;
    }
    else if(type == "spritesheet")
    {
     	resources[*arg_start] = Type::SHEET;
		return true;
    }
    else if(type == "sprite")
    {
     	resources[*arg_start] = Type::SPRITE;
	    return true;
    }
    else
    {
        //improper type
        return false;
    }
}

bool ResourceManager::source(std::string* arg_start, std::string* arg_end)
{
    std::ifstream file;
    std::string line;
    for(auto itr = arg_start; itr != arg_end; ++itr)
    {
        file.open(parentDirectoryPath + (*itr));
        while(std::getline(file, line))
            instructions.push(split(line));    
        file.close();
    }
    return file;       
}

bool ResourceManager::dimension(std::string* arg_start, std::string* arg_end)
{
	if((resources.find(*arg_start) == resources.end()) || ((arg_start + 2) != (arg_end -1)))
	{
		//resource not yet created or improper number of arguments
		return false;
	}
	else if(resources[*arg_start] == Type::SHEET)
	{
		setOfFrames[*arg_start].setDimensions(std::stoi(*(arg_start + 1)), std::stoi(*(arg_start + 2)));
		return true;
	}
	else if(resources[*arg_start] == Type::ANIMATED)
	{
		setOfAnimations[*arg_start].setDimensions(std::stoi(*(arg_start + 1)), std::stoi(*(arg_start + 2)));
		return true;
	}
	else
	{
		//resource not of dimensionable type
		return false;
	}
}

bool ResourceManager::texture(std::string* arg_start, std::string* arg_end)
{
	if((resources.find(*arg_start) == resources.end()) || (((arg_start + 1) != (arg_end -1)) && (((arg_start + 2) != (arg_end -1)))))
	{
		return false;
	}
	else if(resources[*arg_start] == Type::SHEET || resources[*arg_start] == Type::ANIMATED || resources[*arg_start] == Type::SPRITE)
	{	
		textures[*arg_start].setFileName(parentDirectoryPath + *(arg_start + 1));
		if((arg_start + 2) == (arg_end - 1) && ((*(arg_start + 2))[0] == 's'))
			textures[*arg_start].texture.setSmooth(true);
		return true;
	}
	else
	{
		//resource not of texturable type
		return false;
	}
}

bool ResourceManager::add(std::string* arg_start, std::string* arg_end)
{
	if((resources.find(*arg_start) == resources.end()) || (arg_start + 1) == (arg_end - 1))
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

/// Initializes the text keywords associated with each type and command
void ResourceManager::initializeKeywords()
{
    commandMap["add"] = Command::ADD;
    commandMap["#"] = Command::COMMENT;
    commandMap["establish"] = Command::ESTABLISH;
    commandMap["dimension"] = Command::DIMENSION;
    commandMap["source"] = Command::SOURCE;
    commandMap["texture"] = Command::TEXTURE;

    typeMap["animated"] = Type::ANIMATED;
    typeMap["sheet"] = Type::SHEET;
    typeMap["sprite"] = Type::SPRITE;
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

std::vector<std::string> ResourceManager::split(const std::string& line)
{
    // A stringstream will read in separate words and discard whitespace
    std::istringstream parser(line);
    
    // Fill a vector with every word between the start and end of the stream
    std::vector<std::string> words{std::istream_iterator<std::string>(parser),
                                   std::istream_iterator<std::string>()};

    return words;
}

/*
ResourceManager::Instruction::Instruction() {}

ResourceManager::Instruction::Instruction(const std::string& line)
{

    std::stringstream lineStream;
    lineStream.str(line);

    arg_length = 1;
    for(int i = 0; i < (line.length() - 1); i++)
    {
        if((line[i] == ' ') && (line[i + 1] != ' '))
            arg_length++;
    }
    
    arguments = new std::string[arg_length];
    std::string word;

    for(int i = 0; lineStream >> word; ++i)
    {
        arguments[i] = word;   
    }

    command = arguments[0][0];

}

std::ostream& operator<<(std::ostream& out, ResourceManager::Instruction ins)
{

	for(int i = 0; i < ins.arg_length; ++i)
		out << ins.arguments[i] << " ";
	out << std::endl;
	return out;

}
*/

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
