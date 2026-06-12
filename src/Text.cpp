#include "../include/Text.hpp"

Text::Text(const Font& fontRef, std::string text, 
        float x, float y, float scale, float r, float g, float b) 
    : font(fontRef), text(text), x(x), y(y), scale(scale), r(r), g(g), b(b) {}

void Text::draw(Renderer& renderer) {
    if (!font.isLoaded()) return;

    float x_offset = x;
    const auto& fontCharacters = font.getCharacters();

    // Iterate through character strings to render glyphs iteratively
    for (const char& c : text) {
        auto it = fontCharacters.find(c);
        if (it == fontCharacters.end()) continue;

        Character ch = it->second;

        // Perform layout baseline calculation metric conversions
        float xpos = x_offset + ch.Bearing.x * scale;
        float ypos = y + (ch.Size.y - ch.Bearing.y) * scale;
        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;

        renderer.drawTexture(ch.TextureID, xpos, ypos - h, w, h, glm::vec4(r, g, b, 1.0f));

        // Advance baseline position forward (Bitshift by 6 to convert 1/64th pixels to raw units)
        x_offset += (ch.Advance >> 6) * scale;
    }
}

float Text::getTextWidth() const {
    float width = 0.0f;
    const auto& fontCharacters = font.getCharacters();
    for (const char& c : text) {
        auto it = fontCharacters.find(c);
        if (it != fontCharacters.end()) {
            // Bitshift by 6 to add advance cursor length scale logic
            width += (it->second.Advance >> 6) * scale;
        }
    }
    return width;
}

float Text::getTextHeight() const {
    float max_ascent = 0.0f;
    float max_descent = 0.0f;
    const auto& fontCharacters = font.getCharacters();

    for (const char& c : text) {
        auto it = fontCharacters.find(c);
        if (it != fontCharacters.end()) {
            const Character& ch = it->second;
            float ascent = ch.Bearing.y * scale;
            if (ascent > max_ascent) max_ascent = ascent;

            float descent = (ch.Size.y - ch.Bearing.y) * scale;
            if (descent > max_descent) max_descent = descent;
        }
    }
    return max_ascent + max_descent;
}
