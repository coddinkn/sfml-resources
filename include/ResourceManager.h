#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <iostream>
#include <string>
#include <vector>
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
    void reportErrors(std::ostream& out);
private:
    std::string parentDirectory;
    bool valid;
    std::vector<std::string> errorMessages;
    enum Type
    {
        sprite,
        animated,
        sheet
    };
    bool define(const std::string& name, std::vector<std::string>::iterator start, std::vector<std::string>::iterator end);
    std::map<std::string, Type> resources;
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
    Texture defaultTexture;
    Animations defaultAnimations;
    Frames defaultFrames;
    std::map<std::string, Texture> textures;
    std::map<std::string, Animations> setOfAnimations;
    std::map<std::string, Frames> setOfFrames;
};

#endif
