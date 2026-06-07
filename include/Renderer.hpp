#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <array>
#include "../shaders/shader.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoords;
    float texID;
};

class Renderer {
private:
    static const size_t MAX_QUAD_COUNT = 1000;
    static const size_t MAX_VERTEX_COUNT = MAX_QUAD_COUNT * 4;
    static const size_t MAX_INDEX_COUNT = MAX_QUAD_COUNT * 6;
    int maxTextureSlots = 32;

    unsigned int VAO, VBO, EBO;
    Shader quadShader;

    Vertex* bufferBase = nullptr;
    Vertex* bufferPtr = nullptr;
    unsigned int indexCount = 0;

    std::vector<unsigned int> textureSlots;
    unsigned int textureSlotIndex = 1;

    unsigned int whiteTexture;

    void setupBatch();
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    void beginScene(unsigned int width, unsigned int height);
    void endScene();
    void flush();

    void drawRect(float x, float y, float w, float h, glm::vec4 color);
    void drawTexture(unsigned int textureID, float x, float y, float w, float h, glm::vec4 tint = glm::vec4(1.0f));
};
#endif
