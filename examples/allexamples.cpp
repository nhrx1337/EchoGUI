#include "include/EchoGUI.hpp"
#include "include/Renderer.hpp"
#include "include/Font.hpp"
#include "include/Text.hpp"
#include "include/Button.hpp"

int global = 0;

// Simple click callback function
void testFunc() {
    std::cout << "Button clicked! Count: " << global++ << '\n';
}

int main() {
    // Initialize core system and configure window
    EchoGUI mainGui;
    mainGui.scaleMode = WindowScaleMode::STRETCH;
    mainGui.setBackgroundColor(0.1f, 0.1f, 0.1f);
    
    if (mainGui.create_window("EchoGUI - Showcase Example", 800, 600) != 0) {
        return -1;
    }

    // Create the master rendering subsystem
    Renderer renderer;

    // Load font asset (TrueType / OpenType supported)
    Font openSans("./fonts/Anonymous.ttf", 48);

    // Construct UI elements
    Button button(openSans, 300.0f, 250.0f, 200.0f, 60.0f, "Button", testFunc, 0.8f, 0.2f, 0.2f);
    button.setHoverColor(1.0f, 0.3f, 0.3f);
    button.setPressedColor(0.5f, 0.1f, 0.1f);

    Text text(openSans, "test", 0.0f, 48.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    // Center the text layout horizontally based on window dimensions
    text.x = (mainGui.getWindowWidth() - text.getTextWidth()) / 2.0f;

    // Optional layout elements (Uncomment to use)
    // Rect rect(0.0f, 0.0f, 50.0f, 50.0f, 0.0f, 1.0f, 0.0f);
    // Image img("./test.png", 0.0f, 0.0f, 50.0f, 50.0f);

    // Setup runtime logic loop wrapper
    auto update = [&]() {
        // Global key polling
        if (Input::isKeyPressed(Key::ESCAPE)) {
            mainGui.close();
        }

        // Fetch virtual space cursor positioning to update visual layouts
        glm::vec2 mousePos = mainGui.getMousePosition();
        button.update(mousePos.x, mousePos.y, mainGui);
    };

    // Setup rendering loop wrapper
    auto draw = [&]() {
        // Clear framebuffer and bind projection matrix setup
        renderer.beginScene(EchoGUI::getWindowWidth(), EchoGUI::getWindowHeight());

        // Draw components chronologically onto batch queue
        // img.draw(renderer);
        text.draw(renderer);
        button.draw(renderer);

        // Terminate scene frame context and submit geometry data to the GPU
        renderer.endScene();
    };

    // Start the main blocking event framework
    mainGui.run(update, draw);

    return 0;
}
