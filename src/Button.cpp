#include "../include/Button.hpp"

Button::Button(const Font& font,
        float x, float y, float w, float h, std::string textStr, 
        std::function<void()> callback,
        float r, float g, float b) : 
            x(x), y(y), width(w), height(h), 
            callback(callback), state(ButtonState::NORMAL),
            background(x, y, w, h, r, g, b), 
            label(font, textStr, x + 15.0f, y + (h / 2.0f) + 10.0f, 0.5f, 1.0f, 1.0f, 1.0f) {
    float normalColor[] = {r, g, b};
    // Initialize state colors with the default normal color
    for (int rgb = 0; rgb < 3; rgb++) {
        normalC[rgb] = hoverC[rgb] = pressedC[rgb] = normalColor[rgb];
    }
}

void Button::update(float mouseX, float mouseY, EchoGUI& gui) {
    bool isMousePressed = Input::isMouseButtonPressed(MouseButton::LEFT);
    ButtonState oldState = state;

    // Check if mouse cursor is inside the button bounds (AABB Collision)
    if (!(mouseX >= x && mouseX <= (x + width) &&
                  mouseY >= y && mouseY <= (y + height))) {
        state = ButtonState::NORMAL;
    } else {
        if (isMousePressed) {
            state = ButtonState::PRESSED;
        } else {
            // Trigger callback on button release (click event)
            if (oldState == ButtonState::PRESSED && callback) 
                callback();
            state = ButtonState::HOVER;
        }
    }

    // Only request a re-render if the button visual state changes
    if (state == oldState) return;
    switch (state) {
        case ButtonState::NORMAL:
            background.setColor(normalC[0], normalC[1], normalC[2]);
            break;
        case ButtonState::HOVER:
            background.setColor(hoverC[0], hoverC[1], hoverC[2]);
            break;
        case ButtonState::PRESSED:
            background.setColor(pressedC[0], pressedC[1], pressedC[2]);
            break;
    }
    gui.requestRender();
}

void Button::setHoverColor(float r, float g, float b) {
    hoverC[0] = r; hoverC[1] = g; hoverC[2] = b;
}

void Button::setPressedColor(float r, float g, float b) {
    pressedC[0] = r; pressedC[1] = g; pressedC[2] = b;
}

void Button::draw(Renderer& renderer) {
    background.draw(renderer);
    label.draw(renderer);
}

bool Button::isPressed() const { 
    return state == ButtonState::PRESSED; 
}
