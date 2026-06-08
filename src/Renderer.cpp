#include "../include/Renderer.hpp"

Renderer::Renderer() : quadShader("shaders/shader.vs", "shaders/shader.fs") {
    // Query maximum concurrent texture slots supported by user's GPU hardware
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureSlots);
    
    if (maxTextureSlots < 8) maxTextureSlots = 8; 

    textureSlots.resize(maxTextureSlots, 0);

    setupBatch();
    
    // Generate a 1x1 white texture pixel used for drawing solid colored rectangles
    glGenTextures(1, &whiteTexture);
    glBindTexture(GL_TEXTURE_2D, whiteTexture);
    unsigned int whiteData = 0xffffffff;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &whiteData);
    
    textureSlots[0] = whiteTexture; // Reserved slot 0 for primitive shape color rendering

    quadShader.use();
    
    std::vector<int> samplers(maxTextureSlots);
    for (int i = 0; i < maxTextureSlots; i++) {
        samplers[i] = i;
    }

    glUniform1iv(glGetUniformLocation(quadShader.ID, "uTextures"), maxTextureSlots, samplers.data());
}

void Renderer::setupBatch() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // Vertex Layout Attributes Assignment
    glEnableVertexAttribArray(0); // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1); // Color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(2); // TexCoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(3); // TexID
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texID));

    // Pre-calculate indices mapping order for indices buffer element allocation
    unsigned int indices[MAX_INDEX_COUNT];
    unsigned int offset = 0;
    for (size_t i = 0; i < MAX_INDEX_COUNT; i += 6) {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;
        offset += 4;
    }

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    bufferBase = new Vertex[MAX_VERTEX_COUNT];
}

void Renderer::beginScene(unsigned int width, unsigned int height) {
    // Generate screenspace 2D Orthographic Projection Matrix (Y-down coordinate setup)
    glm::mat4 projection = glm::ortho(0.0f, (float)width, (float)height, 
                                                        0.0f, -1.0f, 1.0f);
    quadShader.use();
    glUniformMatrix4fv(glGetUniformLocation(quadShader.ID, "projection"), 
                                1, GL_FALSE, glm::value_ptr(projection));

    // Reset local batch processing markers
    bufferPtr = bufferBase;
    indexCount = 0;
    textureSlotIndex = 1;
}

void Renderer::drawRect(float x, float y, float w, float h, glm::vec4 color) {
    if (indexCount + 6 >= MAX_INDEX_COUNT) flush();

    float texID = 0.0f; // Uses index 0 (white texture pixel)

    bufferPtr->position = { x, y, 0.0f };
    bufferPtr->color = color;
    bufferPtr->texCoords = { 0.0f, 0.0f };
    bufferPtr->texID = texID;
    bufferPtr++;

    bufferPtr->position = { x + w, y, 0.0f };
    bufferPtr->color = color;
    bufferPtr->texCoords = { 1.0f, 0.0f };
    bufferPtr->texID = texID;
    bufferPtr++;

    bufferPtr->position = { x + w, y + h, 0.0f };
    bufferPtr->color = color;
    bufferPtr->texCoords = { 1.0f, 1.0f };
    bufferPtr->texID = texID;
    bufferPtr++;

    bufferPtr->position = { x, y + h, 0.0f };
    bufferPtr->color = color;
    bufferPtr->texCoords = { 0.0f, 1.0f };
    bufferPtr->texID = texID;
    bufferPtr++;

    indexCount += 6;
}

void Renderer::drawBorder(float x, float y, float w, float h, float thickness, glm::vec4 color) {
    if (indexCount + 6 >= MAX_INDEX_COUNT) flush();

    float texID = 0.0f; // Uses index 0 (white texture pixel)
    
    // Normalize thickness relative to the smallest quad dimension to avoid distortion
    float minDim = (w < h) ? w : h;
    float thicknessRatio = thickness / minDim;

    // Pack the thickness ratio into negative alpha to pass data directly to the shader
    glm::vec4 borderData = glm::vec4(color.r, color.g, color.b, -thicknessRatio);

    // Append quad vertices to the batch buffer
    bufferPtr->position = { x, y, 0.0f };
    bufferPtr->color = borderData;
    bufferPtr->texCoords = { 0.0f, 0.0f };
    bufferPtr->texID = texID;
    bufferPtr++;

    bufferPtr->position = { x + w, y, 0.0f };
    bufferPtr->color = borderData;
    bufferPtr->texCoords = { 1.0f, 0.0f };
    bufferPtr->texID = texID;
    bufferPtr++;

    bufferPtr->position = { x + w, y + h, 0.0f };
    bufferPtr->color = borderData;
    bufferPtr->texCoords = { 1.0f, 1.0f };
    bufferPtr->texID = texID;
    bufferPtr++;

    bufferPtr->position = { x, y + h, 0.0f };
    bufferPtr->color = borderData;
    bufferPtr->texCoords = { 0.0f, 1.0f };
    bufferPtr->texID = texID;
    bufferPtr++;

    indexCount += 6;
}

void Renderer::drawTexture(unsigned int textureID, float x, float y, float w, float h, glm::vec4 tint) {
    // Check if texture is already bound within active texture unit queue
    float slot = -1.0f;
    for (unsigned int i = 0; i < textureSlotIndex; i++) {
        if (textureSlots[i] == textureID) {
            slot = (float)i;
            break;
        }
    }

    // Force draw flush if maximum texture layout allocation limits are reached
    if (slot == -1.0f) {
        if (textureSlotIndex >= (unsigned int)maxTextureSlots) 
            flush();
        slot = (float)textureSlotIndex;
        textureSlots[textureSlotIndex] = textureID;
        textureSlotIndex++;
    }

    // Append quad vertices to the batch buffer
    bufferPtr->position = { x, y, 0.0f };
    bufferPtr->color = tint;
    bufferPtr->texCoords = { 0.0f, 0.0f };
    bufferPtr->texID = slot;
    bufferPtr++;

    bufferPtr->position = { x + w, y, 0.0f };
    bufferPtr->color = tint;
    bufferPtr->texCoords = { 1.0f, 0.0f };
    bufferPtr->texID = slot;
    bufferPtr++;

    bufferPtr->position = { x + w, y + h, 0.0f };
    bufferPtr->color = tint;
    bufferPtr->texCoords = { 1.0f, 1.0f };
    bufferPtr->texID = slot;
    bufferPtr++;

    bufferPtr->position = { x, y + h, 0.0f };
    bufferPtr->color = tint;
    bufferPtr->texCoords = { 0.0f, 1.0f };
    bufferPtr->texID = slot;
    bufferPtr++;

    indexCount += 6;
}

void Renderer::flush() {
    if (indexCount == 0) return;

    // Upload batch vertex data to GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (uint8_t*)bufferPtr - (uint8_t*)bufferBase, bufferBase);
    glBindVertexArray(VAO);

    // Bind all active textures to their respective hardware slots
    for (unsigned int i = 0; i < textureSlotIndex; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureSlots[i]);
    }

    // Execute batch draw call
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    // Reset batch state for the next pass (Slot 0 remains reserved for white texture)
    indexCount = 0;
    bufferPtr = bufferBase;
    textureSlotIndex = 1; 
}

void Renderer::endScene() {
    flush(); // Push remaining components to clear screen buffer
}

Renderer::~Renderer() {
    delete[] bufferBase;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &whiteTexture);
}
