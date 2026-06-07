#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Renderer.hpp"

class Image {
    private:
        float x, y, w, h;
        unsigned int texture;
        void loadTexture(const char* filepath);

    public:
        Image(const char* filepath, float x, float y, float w, float h);
        void draw(Renderer& renderer);
};

#endif // IMAGE_HPP
