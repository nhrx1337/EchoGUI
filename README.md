# EchoGUI

## Overview
- EchoGUI is a lightweight, GPU-accelerated graphical user interface library based on OpenGL.

## Installation & Usage
1. Clone the repository:
```bash
git clone https://github.com/nhrx1337/EchoGUI.git
cd EchoGUI/
```

2. Include in your code:
```cpp
// On top of your main source file
#include "include/EchoGUI.hpp"
// Or include specific elements as needed: Button.hpp, Text.hpp, Font.hpp...
```

3. Build:
- You can use the provided Makefile.example
```bash
mv Makefile.example Makefile
make
```
- !Do not forget to change the target source file name inside Makefile.example

## Documents
### EchoGUI (Core System)
- Manages the window lifestyle, scaling behavior, and application states.

```cpp
#include "include/EchoGUI.hpp"

EchoGUI app;

// Supported scaling modes: WindowScaleMode::STRETCH, EXPAND, LETTERBOX
app.scaleMode = WindowScaleMode::LETTERBOX;
app.setBackgroundColor(0.15f, 0.15f, 0.15f);

// Create the window (Returns 0 on success, -1 on failure)
if (app.create_window("EchoGUI Application", 800, 600) != 0) return -1;

// Triggers an application window shutdown
app.close();
```

### Renderer
- Handles screen projection and batches all visual element draws.
```cpp
#include "include/Renderer.hpp"

Renderer renderer;

// Prepares the frame with current window bounds
renderer.beginScene(EchoGUI::getWindowWidth(), EchoGUI::getWindowHeight());

// Directly draws a raw vector-colored primitive rectangle
renderer.drawRect(50.0f, 50.0f, 200.0f, 100.0f, glm::vec4(0.2f, 0.6f, 0.8f, 1.0f));

// Finishes the batch and pushes drawing elements to the GPU
renderer.endScene();
```

### Font & Text
- Loads TrueType font textures and displays calibrated text layouts.
```cpp
#include "include/Font.hpp"
#include "include/Text.hpp"

// Load a specific .ttf file with pixel size
Font myFont("assets/fonts/Roboto.ttf", 32);

// Text constructor: Font, string, X, Y, Scale, R, G, B
Text label(myFont, "Test", 100.0f, 100.0f, 1.0f, 1.0f, 1.0f, 1.0f);

// Renders the text characters iteratively
label.draw(renderer);

// Returns the calculated layout pixel width of the text string
float textWidth = label.getTextWidth();
```

### Button
- Handles mouse bounding-box collision updates and triggers a callback upon release.
```cpp
#include "include/Button.hpp"

// Define a simple execution callback
auto onClick = []() { std::cout << "Clicked!\n"; };

// Button constructor: Font, X, Y, Width, Height, Label, Callback, Default RGB Color
Button btn(myFont, 300.0f, 250.0f, 200.0f, 50.0f, "Click Me", onClick, 0.2f, 0.2f, 0.2f);

// Customize operational visual colors
btn.setHoverColor(0.3f, 0.5f, 0.8f);
btn.setPressedColor(0.1f, 0.3f, 0.6f);

// Update routine checking collision boundaries (AABB) using UI space coordinates
btn.update(mouseX, mouseY, app);

// Renders the solid background rectangle along with its inner text
btn.draw(renderer);
```

### Rect
- A standalone shape helper wrapper for primitive drawing operations.
```cpp
#include "include/Rect.hpp"

// Rect constructor: X, Y, Width, Height, R, G, B
Rect box(100.0f, 400.0f, 150.0f, 75.0f, 0.8f, 0.2f, 0.2f);

// Modify layout color parameters dynamically
box.setColor(0.2f, 0.8f, 0.2f);

// Draws the shape utilizing active batch queues
box.draw(renderer);
```

### Image
- Loads external asset textures onto layout dimensions.
```cpp
#include "include/Image.hpp"

// Image constructor: Filepath, X, Y, Width, Height
Image logo("assets/images/logo.png", 10.0f, 10.0f, 64.0f, 64.0f);

// Renders image texture file onto specified bounding box coordinates
logo.draw(renderer);
```

### TextField
- Creates an interactive text input field with dynamic scrolling.
```cpp
#include "include/TextField.hpp"
#include "include/Font.hpp"

// Load font asset first
Font Arial("assets/fonts/<font>.ttf", 24);

// TextField constructor: Font, X, Y, Width, Height
TextField txtf(Arial, 10.0f, 10.0f, 200.0f, 40.0f);

// Update logic inside the main loop (handles focus, mouse clicks, and keyboard inputs)
txtf.update(mouseX, mouseY, gui);

// Renders background, dynamic border, and clipped text area
txtf.draw(renderer, gui);

// Retrieve the typed text string when needed
std::string text = txtf.getString();
```

### Input Handling

- A global static utility providing simple peripheral polling.
```cpp
#include "include/Input.hpp"

// Query explicit hardware keyboard inputs
if (Input::isKeyPressed(Key::ESCAPE)) {
    app.close();
}

// Query explicit hardware pointer buttons
if (Input::isMouseButtonPressed(MouseButton::LEFT)) {
    std::cout << "Left mouse button is held down!\n";
}
```
