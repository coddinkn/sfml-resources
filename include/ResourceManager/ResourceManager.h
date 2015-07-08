#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
	ResourceManager(const std::string& resourceFilePath);
	const sf::Texture& getTexture(const std::string& name);
	const std::map<std::string, sf::IntRect>& getFrames(const std::string& name);
	const std::map<std::string, std::vector<sf::IntRect>>& getAnimations(const std::string& name);
	operator bool();

private:
    enum Command
    {
        ADD_ANIMATION, ADD_FRAME, COMMENT, DIMENSION, SOURCE,
        TEXTURE, INVALID_COMMAND, START_BLOCK, END_BLOCK, TYPE
    };
    enum Type { ANIMATED, SHEET, SPRITE, INVALID_TYPE }; 
    enum Element { ANIMATION, FRAME, INVALID_ELEMENT };

    void initializeKeywords(); 

    std::map<std::string, Command> commandMap;
    std::map<std::string, Type> typeMap;
    Command parseCommand(std::string command);
    Type parseType(std::string type);

    std::map<Command, int> argumentNumbers;
    bool checkArgumentNumber(Command, int);
	bool checkScope(Command, bool inside_block);   
 
    std::map<std::string, Type> resources;
    std::string parentDirectoryPath;
    bool valid;

    std::vector<std::vector<std::string>> instructions;
    std::vector<std::string> split(const std::string& line);

    void source(const std::vector<std::string>& arguments, std::string& error);
    void dimension(const std::string& resourceName, const std::vector<std::string>& arguments, std::string& error);
    void texture(const std::string& resourceName, const std::vector<std::string>& arguments, std::string& error);
    void newAnimation(const std::string& resourceName, const std::vector<std::string>& arguments, std::string& error);
    void newFrame(const std::string& resourceName, const std::vector<std::string>& arguments, std::string& error);   
 
    struct Texture
    {
        Texture();
        bool loaded = false;
		bool load();
        std::string fileName;
        void setFileName(const std::string& fileName);
        sf::Texture texture;   
    };

    struct Frames
    {
        Frames();
        int width;
        int height;
        bool dimensioned = false;
        void setDimensions(int width, int height);
        void addFrame(const std::string& frameName, int x, int y);
        std::map<std::string, sf::IntRect> frames;
    };

    struct Animations
    {
        Animations();
        int width;
        int height;
        bool dimensioned = false;
        void setDimensions(int width, int height);
        void addAnimation(const std::string& animationName, int x, int y, bool isVertical, int frames);
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
