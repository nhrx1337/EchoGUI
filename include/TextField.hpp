#ifndef TEXTFIELD_HPP
#define TEXTFIELD_HPP

#include "Text.hpp"
#include "EchoGUI.hpp"

class TextField {
private:
    float x, y, w, h, thickness = 1.0f;
    float bg_r = 0.15f, bg_g = 0.15f, bg_b = 0.15f;
    
    std::string input_text = "";
    Text text;
    
    float scrollOffset = 0.0f;
    float maxVisibleWidth = 0.0f;

    bool isFocused = false;
public:
    TextField(const Font& font, float x, float y, float w, float h);

    void update(float mouseX, float mouseY, EchoGUI& gui);
    void draw(Renderer& renderer, EchoGUI& gui);

    std::string getString();
    void setThickness(float t);
    void setTextColor(float r, float g, float b);
    void setBackgroundColor(float r, float g, float b);
};

#endif // TEXTFIELD_HPP
