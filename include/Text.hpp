#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include "Renderer.hpp"
#include "Font.hpp"

class Text {
    private:
        const Font& font;
    public:
        std::string text;
        float x, y, scale, r, g, b;

        Text(const Font& fontRef, std::string text, float x, float y, float scale, float r, float g, float b);

        float getTextWidth() const;
        void draw(Renderer& renderer);
};

#endif // TEXT_HPP
