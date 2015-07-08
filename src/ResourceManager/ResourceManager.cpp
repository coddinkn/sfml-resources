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
	parentDirectoryPath = "";
	size_t lastSeparator = resourceFilePath.find_last_of('/');
	if (lastSeparator != std::string::npos) 
		parentDirectoryPath = resourceFilePath.substr(0, lastSeparator + 1);

	// Attempt to open the resource file
	std::ifstream file(resourceFilePath);
	if (!file)
	{
		valid = false;
		return;   
	}

	// Read in each instruction from the resource file
	std::vector<std::vector<std::string>> instructions;
	std::string line;
	while (std::getline(file, line))
	{
		std::vector<std::string> arguments = split(line);
		if (arguments.size() > 0) // Ignore empty/whitespace-only lines
			instructions.push_back(arguments);
	}
	file.close();

	// Parse and process each command
	int lineNumber = 1;
	std::string error = "";
	std::string current_resource = "";
	bool inside_block = false;
	while (instructions.size() != 0)
	{
		std::vector<std::string> args = instructions.front();
		Command command = parseCommand(args[0]);

		// Ensure that the correct number of arguments are given
		if (checkArgumentNumber(command, args.size()) && checkScope(command, inside_block))
		{
			switch (command)
			{
				case Command::START_BLOCK:
					inside_block = true;
					break;
				case Command::END_BLOCK:
					inside_block = false;
					break;
				case Command::DEFINE:
					current_resource = args[1];
					define_resource(current_resource, parseType(args[0]), error);
					break;
				case Command::ADD_ANIMATION:
					newAnimation(current_resource, args[1], std::stoi(args[2]), std::stoi(args[3]), std::stoi(args[4]), args[5], error);
					break;
				case Command::ADD_FRAME:
					newFrame(current_resource, args[1], std::stoi(args[2]), std::stoi(args[3]), error);
					break;
				case Command::DIMENSION:
					dimension(current_resource, std::stoi(args[1]), std::stoi(args[2]), error);
					break;
				case Command::SOURCE:
					file.open(parentDirectoryPath + args[0]);
					if (!file)
					{
						error = "failed to open " + parentDirectoryPath + args[0];
					}
					else // Non-error case
					{
						while (std::getline(file, line))
							instructions.push_back(split(line));    
					}
					file.close();
					break;
				case Command::TEXTURE:
					texture(current_resource, args, error);
					break;
				default:
					error = "unrecognized command \"" + args[0] + "\"";
					break;
			}
		}
		else
		{
			error = "invalid number of arguments for command \"" + args[0]
				+ "\"";
		}
		// Print out error information if an error occurred
		if (error != "")
		{
			std::cerr << "[ResourceManager]: " << error << ':' << std::endl;
			std::cerr << "\tline " << lineNumber << ":";
			for (unsigned i = 0; i < args.size(); ++i)
				std::cerr << ' ' << args[i];
			std::cerr << std::endl;
			valid = false;
		}
		// Clean up and reset error status
		instructions.erase(instructions.begin());
		lineNumber++;
		error = "";
	}
}

///// Command processing /////

void ResourceManager::define_resource(const std::string& resourceName, ResourceManager::Type type, std::string& error)
{

	if (resources.find(resourceName) != resources.end())
	{
		error = "cannot define resource \"" + resourceName
			+ "\", it has already been defined";
	}
	if (type == Type::INVALID_TYPE)
	{
		error = "invalid type specified";
	}
	else // Non-error case
	{
		resources[resourceName] = type;
	}
}

/// Updates the dimensions of the frames within a sprite sheet or animation
void ResourceManager::dimension(const std::string& resourceName, int width, int height, std::string& error)
{
	if (resources.find(resourceName) == resources.end())
	{
		error = "resource \"" + resourceName + "\" does not exist";
	}
	else if (resources[resourceName] == Type::SHEET)
	{
		setOfFrames[resourceName].setDimensions(width, height);
	}
	else if (resources[resourceName] == Type::ANIMATED)
	{
		setOfAnimations[resourceName].setDimensions(width, height);
	}
	else
	{
		error = "resource \"" + resourceName + "\" is not of dimensionable type";
	}
}

/// Links a texture file to a given resource
void ResourceManager::texture(const std::string& resourceName, const std::vector<std::string>& args, std::string& error)
{
	if (resources.find(resourceName) == resources.end())
	{
		error = "resource \"" + resourceName + "\" does not exist";
	}
	else if (resources[resourceName] == Type::INVALID_TYPE)
	{
		error = "invalid type specified";
	}
	else if (args.size() == 3 && args[2] != "smooth" && args[2] != "rough")
	{
		error = "unrecognized filtering flag \"" + args[2]  + "\"";
	}
	else // Non-error case
	{
		textures[resourceName].setFileName(parentDirectoryPath + args[1]);
		if (args.size() == 3 && args[2] == "smooth")
			textures[resourceName].texture.setSmooth(true);
	}
}

/// Adds a new animation to a given animated sprite
void ResourceManager::newAnimation(const std::string& resourceName, const std::string& animationName, int x, int y, int frameCount, const std::string& direction, std::string& error)
{
	if (resources.find(resourceName) == resources.end())
	{
		error = "resource \"" + resourceName + "\" does not exist";
	}
	else if (resources[resourceName] != Type::ANIMATED)
	{
		error = "\"" + resourceName + "\" is not an animated sprite";
	}
	else if (setOfAnimations[resourceName].animations.find(animationName)
			!= setOfAnimations[resourceName].animations.end())
	{
		error = "animation \"" + animationName
			+ "\" is alread present in the group \"" + resourceName + "\"";
	}
	else if (direction != "h" && direction != "v")
	{
		error = "unrecognized direction flag \"" + direction + "\"";
	}
	else
	{
		bool vertical = (direction == "v");

		setOfAnimations[resourceName].addAnimation(animationName, x, y,
				vertical, frameCount);
	}
}

/// Adds a new frame to a given sprite sheet
void ResourceManager::newFrame(const std::string& resourceName, const std::string& frameName, int x, int y, std::string& error)
{
	if (resources.find(resourceName) == resources.end())
	{
		error = "resource \"" + resourceName + "\" does not exist";
	}
	else if (resources[resourceName] != Type::SHEET)
	{
		error = "\"" + resourceName + "\" is not a sprite sheet";
	}
	else if (setOfFrames[resourceName].frames.find(frameName)
			!= setOfFrames[resourceName].frames.end())
	{
		error = "frame \"" + frameName
			+ "\" is alread present in the group \"" + resourceName + "\"";
	}
	else // Non-error case
	{
		setOfFrames[resourceName].addFrame(frameName, x, y);
	}
}

///// Parsing functions /////

/// Initializes the text keywords associated with each type and command
void ResourceManager::initializeKeywords()
{
	// Note: the "Comment" command is handled in parseCommand as a special case
	commandMap["add-animation"] = Command::ADD_ANIMATION;
	commandMap["add-frame"] = Command::ADD_FRAME;
	commandMap["dimension"] = Command::DIMENSION;
	commandMap["source"] = Command::SOURCE;
	commandMap["texture"] = Command::TEXTURE;
	commandMap["{"] = Command::START_BLOCK;
	commandMap["}"] = Command::END_BLOCK;
	commandMap["sprite"] = Command::DEFINE;
	commandMap["sheet"] = Command::DEFINE;
	commandMap["animated"] = Command::DEFINE;

	typeMap["animated"] = Type::ANIMATED;
	typeMap["sheet"] = Type::SHEET;
	typeMap["sprite"] = Type::SPRITE;

	argumentNumbers[Command::DEFINE] = 2;
	argumentNumbers[Command::ADD_ANIMATION] = 6;
	argumentNumbers[Command::ADD_FRAME] = 4;
	argumentNumbers[Command::DIMENSION] = 3;
	argumentNumbers[Command::SOURCE] = 2;
}

/// Finds the command associated with a given input string
ResourceManager::Command ResourceManager::parseCommand(std::string command)
{
	// Search for the command within the map
	auto matchedCommand = commandMap.find(command);

	// If the search fails, check if the line is a comment. Otherwise, return
	// INVALID. If the search succeeds, return the associated command
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

/// Verifies whether the number of arguments is correct for a given command
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

bool ResourceManager::checkScope(Command command, bool in_block)
{
	switch(command)
	{
		case Command::START_BLOCK:
			return !in_block;	
		case Command::END_BLOCK:
			return in_block;
		case Command::DEFINE:
			return !in_block;
		case Command::ADD_ANIMATION:
			return in_block;
		case Command::ADD_FRAME:
			return in_block;
		case Command::DIMENSION:
			return in_block;
		case Command::SOURCE:
			return !in_block;
		case Command::TEXTURE:
			return in_block;
		default:
			//this should seriously never ever happen
			std::cerr << "[ResourceManager]: fatal error" << std::endl;
			return false;
	}
}


/// Separates the words within a line, removing all whitespace
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

/// Associates a texture with a given file name
void ResourceManager::Texture::setFileName(const std::string& fileName)
{
	this->fileName = fileName;
}

/// Loads in and initializes the texture from a previously provided file name.
/// Returns true if successful
bool ResourceManager::Texture::load()
{
	return (loaded = texture.loadFromFile(fileName));
}

/// Retrieves the sf::Texture associated with a given resource name
const sf::Texture& ResourceManager::getTexture(const std::string& name)
{
	// Attempts to retrieve or load the texture (if not yet loaded)
	if (textures[name].loaded || textures[name].load())
	{
		return textures[name].texture;
	}
	else // Use the default texture if the request can't be completed
	{
		return defaultTexture;
	}
}

///// Animations struct functions /////

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
		return defaultAnimations;
	}
}

///// Frames struct functions /////

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

ResourceManager::operator bool()
{
	return valid;
}
