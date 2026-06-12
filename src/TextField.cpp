#include "../include/TextField.hpp"
#include "../include/Font.hpp"

TextField::TextField(const Font& font, float x, float y, float w, float h) 
    : x(x), y(y), text(font, "", x, y, 
            1.0f, 1.0f, 1.0f, 1.0f) {

    // Ensure minimum dimensions
    if (w < 20.0f) w = 20.0f;
    if (h < 20.0f) h = 20.0f;
    this->w = w;
    this->h = h;

    maxVisibleWidth = w - (6.0f * thickness);

    // Calculate text scale based on field height
    text.text = "A";
    float textHeight = text.getTextHeight();
    float calcScale = (h * 0.5f) / textHeight;

    if (calcScale < 0.1f) {
        calcScale = 0.1f;
    } else if (calcScale > 1.0f) {
        calcScale = 1.0f;
    }
    text.scale = calcScale;

    // Recalculate text dimensions and restore original text
    textHeight = text.getTextHeight();
    text.text = input_text;

    // Center text vertically
    text.x = x + 3*thickness;
    text.y = y + (h/2.0f) + (textHeight / 4.0f);
}

std::string TextField::getString() {
    return input_text;
}

void TextField::setThickness(float t) {
    if (t < 10.0f && t > 0.1f) {
        thickness = t;
        text.x = x + 3*thickness;
        maxVisibleWidth = w - (6.0f * thickness);
    }
}

void TextField::setTextColor(float r, float g, float b) {
    text.r = r;
    text.g = g;
    text.b = b;
}

void TextField::setBackgroundColor(float r, float g, float b) {
    bg_r = r;
    bg_g = g;
    bg_b = b;
}

void TextField::update(float mouseX, float mouseY, EchoGUI& gui) {
    std::string newChars = Input::getAndClearCharBuffer();
    
    bool isClicked = Input::isMouseButtonPressed(MouseButton::LEFT);
    bool oldFocused = isFocused;
    
    // Focus check
    if (isClicked) {
        if ((mouseX >= x && mouseX <= (x + w) &&
            mouseY >= y && mouseY <= (y + h))) {
            isFocused = true;
        } else {
            isFocused = false;
        }
        if (oldFocused != isFocused) {
            gui.requestRender();
        }
    }
    if (!isFocused) {
        return; 
    }

    bool changed = false;
    
    // Handle text input
    if (!newChars.empty()) {
        input_text += newChars;
        text.text = input_text;
        changed = true;
    }

    // Handle backspace
    if (Input::isKeyPressed(Key::BACKSPACE) && !input_text.empty()) {
        input_text.pop_back();
        text.text = input_text;
        changed = true;
    }

    // Update horizontal scrolling if text exceeds bounds
    if (changed) {
        float totalTextWidth = text.getTextWidth();
        
        if (totalTextWidth > maxVisibleWidth) {
            scrollOffset = totalTextWidth - maxVisibleWidth;
        } else {
            scrollOffset = 0.0f;
        }
        gui.requestRender();   
    }
}

void TextField::draw(Renderer& renderer, EchoGUI& gui) {
    // Draw background and border
    renderer.drawRect(x, y, w, h, glm::vec4(bg_r, bg_g, bg_b, 1.0f)); 

    glm::vec4 borderColor = isFocused ? 
        glm::vec4(0.0f, 0.5f, 1.0f, 1.0f) : 
        glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);  

    renderer.drawBorder(x, y, w, h, thickness, borderColor);
    // Flush before enabling OpenGL scissor test
    renderer.flush();

    glEnable(GL_SCISSOR_TEST);

    int winWidth, winHeight;
    glfwGetWindowSize(gui.getWindow(), &winWidth, &winHeight);

    const float base_width = 800.0f;
    const float base_height = 600.0f;

    // Virtual scissor dimensions
    int virScissorX = (int)x + (3.0f * thickness);
    int virScissorY = (int)y; 
    int virScissorW = (int)maxVisibleWidth;
    int virScissorH = (int)h;

    int scissorX = 0, scissorY = 0, scissorW = 0, scissorH = 0;

    // Calculate OpenGL scissor box based on GUI scaling mode
    switch (gui.scaleMode) {
        case WindowScaleMode::STRETCH: {
            float xRatio = (float)winWidth  / base_width;
            float yRatio = (float)winHeight / base_height;
            
            scissorX = (int)(virScissorX * xRatio);
            scissorW = (int)(virScissorW * xRatio);
            scissorH = (int)(virScissorH * yRatio);

            // Invert Y for OpenGL coordinate system
            float screenY = virScissorY * yRatio;
            scissorY = winHeight - (int)(screenY + scissorH);

            break;
        }
        case WindowScaleMode::EXPAND: {
            scissorX = (int)virScissorX;
            scissorW = (int)virScissorW;
            scissorH = (int)virScissorH;

            // Invert Y for OpenGL coordinate system
            scissorY = winHeight - (int)(virScissorY + scissorH);

            break;
        }
        case WindowScaleMode::LETTERBOX: {
            float aspectRatio = base_width / base_height;

            int vpWidth = winWidth;
            int vpHeight = (int)(winWidth / aspectRatio);

            if (vpHeight > winHeight) {
                vpHeight = winHeight;
                vpWidth = (int)(winHeight * aspectRatio);
            }

            float xOffset = (winWidth - vpWidth) / 2.0f;
            float yOffset = (winHeight - vpHeight) / 2.0f;
            
            float scaleX = (float)vpWidth / base_width;
            float scaleY = (float)vpHeight / base_height;

            scissorX = (int)(xOffset + (virScissorX * scaleX));
            scissorW = (int)(virScissorW * scaleX);
            scissorH = (int)(virScissorH * scaleY);
            
            // Invert Y for OpenGL coordinate system
            float screenViewportY = virScissorY * scaleY;
            scissorY = winHeight - (int)(yOffset + screenViewportY + scissorH);

            break;
        }
    }

    glScissor(scissorX, scissorY, scissorW, scissorH);

    // Apply scroll offset and draw text
    text.x = (x + 3.0f * thickness) - scrollOffset;
    text.draw(renderer);
    
    renderer.flush();

    glDisable(GL_SCISSOR_TEST);
}
