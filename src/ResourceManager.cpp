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
            case 'c':
                if(!create(instructions.front().arguments + 1, instructions.front().arguments + instructions.front().arg_length)) std::cerr << instructions.front() << "\terror: improper type or redefinition of resource" << std::endl; 
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
                break;
            default:  
                //invalid command
                break; 
        }

        delete [] instructions.front().arguments;
		instructions.pop();

    }

	for(auto itr = resources.begin(); itr != resources.end(); ++itr)
		std::cout << itr->first << std::endl;

}

bool ResourceManager::create(std::string* arg_start, std::string* arg_end)
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
        resources[*arg_start] = Type::animated;
		return true;
    }
    else if(type == "spritesheet")
    {
     	resources[*arg_start] = Type::sheet;
		return true;
    }
    else if(type == "sprite")
    {
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
	if((resources.find(*arg_start) == resources.end()) || ((arg_start + 2) != (arg_end -1)))
	{
		//resource not yet created or improper number of arguments
		return false;
	}
	else if(resources[*arg_start] == Type::sheet)
	{
		setOfFrames[*arg_start].setDimensions(std::stoi(*(arg_start + 1)), std::stoi(*(arg_start + 2)));
		return true;
	}
	else if(resources[*arg_start] == Type::animated)
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
	else if(resources[*arg_start] == Type::sheet || resources[*arg_start] == Type::animated || resources[*arg_start] == Type::sprite)
	{	
		textures[*arg_start].setFileName(file_path + *(arg_start + 1));
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
	if((resources.find(*arg_start) == resources.end()) || ((arg_start + 2) != (arg_end -1)))
	{
		//resource not yet created or improper number of arguments
		return false;
	}
	else if(resources[*arg_start] == Type::sheet)
	{
		setOfFrames[*arg_start].setDimensions(std::stoi(*(arg_start + 1)), std::stoi(*(arg_start + 2)));
		return true;
	}
	else if(resources[*arg_start] == Type::animated)
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
