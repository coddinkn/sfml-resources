#include "ResourceManager.h"

#include <ostream>
#include <fstream>
#include <sstream>

ResourceManager::ResourceManager(const std::string& res_file_path)
{

	sf::Image defaultImage;
	defaultImage.create(64, 64, sf::Color(0xFF, 0x00, 0xFF));
	defaultTexture.loadFromImage(defaultImage);

	defaultAnimations["null_animation"].push_back(sf::IntRect(0, 0, 64, 64));

	defaultFrames["null_frame"] = sf::IntRect(0, 0, 64, 64);

    size_t lastSeparator = res_file_path.find_last_of('/');
    if(lastSeparator == std::string::npos) 
        file_path = "";
    else
        file_path = res_file_path.substr(0, lastSeparator + 1);
    
    std::ifstream file(res_file_path);

    if(!file)
    {
        valid = false;
        return;   
    }

    std::string line;

    while(std::getline(file, line))
    {
        instructions.push(Instruction(line));            
    }

    while(instructions.size() != 0)
    {
        
        switch(instructions.front().command)
        {
            case '#':            
				//comment
                break;
            case 'e':
                if(!define(instructions.front().arguments + 1, instructions.front().arguments + instructions.front().arg_length)) std::cerr << instructions.front() << "\terror: improper type or redefinition of resource" << std::endl; 
                break;
            case 's':
                if(!source(instructions.front().arguments + 1, instructions.front().arguments + instructions.front().arg_length)) std::cerr << instructions.front() << "\terror: problem opening file(s) being sourced" << std::endl; 
                break;
            case 'd':
                std::cerr << "DEMENSIONING" << std::endl;
                break;
            case 't':
                std::cerr << "TEXTURING" << std::endl;
                break;
            case 'a':
                std::cerr << "ANIMATING" << std::endl;
                break;
            case 'f':
                std::cerr << "FRAMING" << std::endl;
                break;
            default:  
                //invalid command
                break; 
        }

        instructions.pop();

    }

	for(auto itr = resources.begin(); itr != resources.end(); ++itr)
		std::cout << itr->first << std::endl;

}

bool ResourceManager::define(std::string* arg_start, std::string* arg_end)
{
    std::string type = "";
    for(auto itr = (arg_start + 1); itr != arg_end; ++itr)
        type += *itr;

	if(resources.find(*arg_start) != resources.end())
	{
		//resource already defined
		return false;
	}
    else if(type == "animatedsprite") 
    {
        //std::cerr << "animated sprite" << std::endl;
        resources[*arg_start] = Type::animated;
		return true;
    }
    else if(type == "spritesheet")
    {
        //std::cerr << "sprite sheet" << std::endl;
     	resources[*arg_start] = Type::sheet;
	    return true;
    }
    else if(type == "sprite")
    {
        //std::cerr << "sprite" << std::endl;
     	resources[*arg_start] = Type::sprite;
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
        file.open(file_path + (*itr));
        while(std::getline(file, line))
        {
            instructions.push(Instruction(line));    
        }
        file.close();
    }
    return file;       
}

bool ResourceManager::dimension(std::string* arg_start, std::string* arg_end)
{
	return true;
}

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

    if(arguments[0] == "define")
        command = 'e';
    else
        command = arguments[0][0];

}

std::ostream& operator<<(std::ostream& out, ResourceManager::Instruction ins)
{

	for(int i = 0; i < ins.arg_length; ++i)
		out << ins.arguments[i] << " ";
	out << std::endl;
	return out;

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
