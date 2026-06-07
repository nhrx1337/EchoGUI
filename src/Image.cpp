#include "../include/Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb/stb_image.h"

void Image::loadTexture(const char* filepath) {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filepath, &width, &height, &nrChannels, 0);

    if (data) {
        // Detect color space channels dynamically
        GLenum format = GL_RGB;
        if (nrChannels == 1)      format = GL_RED;
        else if (nrChannels == 3) format = GL_RGB;
        else if (nrChannels == 4) format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture\n";
    }
    stbi_image_free(data);
}

Image::Image(const char* filepath, float x, float y, float w, float h) 
    : x(x), y(y), w(w), h(h) {
    loadTexture(filepath);
}

void Image::draw(Renderer& renderer) {
    renderer.drawTexture(this->texture, x, y, w, h);
}
