#ifndef FONT_HPP
#define FONT_HPP

#include <map>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID; 
    glm::ivec2   Size;      
    glm::ivec2   Bearing;   
    unsigned int Advance;   
};

class Font {
private:
    std::map<char, Character> characters;
    bool loaded = false;

public:
    Font() = default;
    Font(const std::string& fontPath, unsigned int fontSize);
    ~Font();

    bool load(const std::string& fontPath, unsigned int fontSize);
    const std::map<char, Character>& getCharacters() const;
    bool isLoaded() const { return loaded; }
};

#endif // FONT_HPP
