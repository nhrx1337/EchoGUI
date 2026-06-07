#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "EchoGUI.hpp"
#include "Rect.hpp"
#include "Text.hpp"
#include "Input.hpp"
#include <string>

enum ButtonState {
    NORMAL,
    HOVER,
    PRESSED
};

class Button {
    private:
        float x, y, width, height;
        std::function<void()> callback;
        ButtonState state;
    
        Rect background;
        Text label;

        float normalC[3];
        float hoverC[3];
        float pressedC[3];
    public:
        Button(const Font& font, float x, float y, float w, float h, std::string textStr, 
        std::function<void()> callback, float r, float g, float b);

        void update(float mouseX, float mouseY, EchoGUI& gui);
        void draw(Renderer& renderer);
        void setHoverColor(float r, float g, float b);
        void setPressedColor(float r, float g, float b);
        bool isPressed() const;
};

#endif // BUTTON_HPP
