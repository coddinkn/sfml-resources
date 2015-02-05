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
    ResourceManager(const std::string& res_file_path);
    const sf::Texture& getTexture(const std::string& name);
    const std::map<std::string, sf::IntRect>& getFrames(const std::string& name);
    const std::map<std::string, std::vector<sf::IntRect>>& getAnimations(const std::string& name);
	operator bool();
private:
    std::string file_path;
    bool valid;
    enum Type{
        sprite,
        sheet,
        animated,
    };
    std::map<std::string, Type> resources;
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
	bool define(std::string* arg_start, std::string* arg_end);
    bool source(std::string* arg_start, std::string* arg_end);
    bool dimension(std::string* arg_start, std::string* arg_end);
    bool texture(std::string* arg_start, std::string* arg_end);
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
