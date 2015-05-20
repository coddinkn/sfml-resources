#ifndef NEW_RESOURCE_MANAGER
#define NEW_RESOURCE_MANAGER

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
	ResourceManager(const std::string& res_parentDirectoryPath);
	const sf::Texture& getTexture(const std::string& name);
	const std::map<std::string, sf::IntRect>& getFrames(const std::string& name);
	const std::map<std::string, std::vector<sf::IntRect>>& getAnimations(const std::string& name);
	operator bool();
private:
    void initializeKeywords(); 

    enum Command
    {
        ADD, COMMENT, ESTABLISH, DIMENSION, SOURCE, TEXTURE, INVALID_COMMAND
    };
    std::map<std::string, Command> commandMap;
    Command parseCommand(std::string command);

    enum Type { ANIMATED, SHEET, SPRITE, INVALID_TYPE }; 
    std::map<std::string, Type> typeMap;
    Type parseType(std::string type);

    std::map<std::string, Type> resources;
    std::string parentDirectoryPath;
    bool valid;

    std::queue<std::vector<std::string>> instructions;
    std::vector<std::string> split(const std::string& line);

    /*
	struct Instruction
    {
	    Instruction();
        Instruction(const std::string& line);
		char command;
        std::string* arguments;
        int arg_length;
    };
    std::queue<Instruction> instructions;
    friend std::ostream& operator<<(std::ostream& out, Instruction ins);
    */

    bool establish(std::string* arg_start, std::string* arg_end);
    bool source(std::string* arg_start, std::string* arg_end);
    bool dimension(std::string* arg_start, std::string* arg_end);
    bool texture(std::string* arg_start, std::string* arg_end);
	bool add(std::string* arg_start, std::string* arg_end); 
    struct Texture
    {
        Texture();
        bool loaded = false;
		bool load();
        std::string file_name;
        void setFileName(const std::string& file_name);
        sf::Texture texture;   
    };
    struct Frames
    {
        Frames();
        int width;
        int height;
        bool dimensioned = false;
        void setDimensions(int width, int height);
        void addFrame(const std::string& frame_name, int x, int y);
        std::map<std::string, sf::IntRect> frames;
    };
    struct Animations
    {
        Animations();
        int width;
        int height;
        bool dimensioned = false;
        void setDimensions(int width, int height);
        void addAnimation(const std::string& animation_name, int x, int y, bool isVertical, int frames);
        std::map<std::string, std::vector<sf::IntRect>> animations; 
    };
    sf::Texture defaultTexture;
    std::map<std::string, sf::IntRect> defaultFrames;
	std::map<std::string, std::vector<sf::IntRect>> defaultAnimations;
	std::map<std::string, Texture> textures;
    std::map<std::string, Animations> setOfAnimations;
    std::map<std::string, Frames> setOfFrames;
};

#endif
