#ifndef RECT_HPP
#define RECT_HPP

#include "Renderer.hpp"

class Rect {
    private:
        float x, y, w, h;
        float r, g, b;
    public:
        Rect(float x, float y, float w, float h, float red, float green, float blue);
        void draw(Renderer& renderer);
        void setColor(float red, float green, float blue);
};

#endif // RECT_HPP
