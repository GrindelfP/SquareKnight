#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <SFML/Graphics.hpp>

class ResourceManager {
private:
    static std::map<std::string, sf::Texture>& getTextures() {
        static std::map<std::string, sf::Texture> textures;
        return textures;
    }

public:
    static sf::Texture& getTexture(const std::string& filename) {
        auto& texMap = getTextures();
        if (texMap.find(filename) == texMap.end()) {
            texMap[filename].loadFromFile(filename);
        }
        return texMap[filename];
    }

    static void clear() {
        getTextures().clear();
    }
};

#endif //RESOURCEMANAGER_H