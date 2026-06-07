#include "../include/Rect.hpp"

Rect::Rect(float x, float y, float w, float h, float red, float green, float blue) 
    : x(x), y(y), w(w), h(h), r(red), g(green), b(blue) {}

void Rect::draw(Renderer& renderer) {
    renderer.drawRect(x, y, w, h, glm::vec4(r, g, b, 1.0f));
}

void Rect::setColor(float red, float green, float blue) {
    r = red;
    g = green;
    b = blue;
}
