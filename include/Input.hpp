#ifndef INPUT_HPP
#define INPUT_HPP

#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class Key {
    ESCAPE = GLFW_KEY_ESCAPE,
    SPACE  = GLFW_KEY_SPACE,
    ENTER  = GLFW_KEY_ENTER,
    BACKSPACE = GLFW_KEY_BACKSPACE
};

enum class MouseButton {
    LEFT   = GLFW_MOUSE_BUTTON_LEFT,
    RIGHT  = GLFW_MOUSE_BUTTON_RIGHT,
    MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

class Input {
private:
    static GLFWwindow* window;
    static std::string charBuffer;

public:
    static void init(GLFWwindow* win);
    static bool isKeyPressed(Key key);
    static bool isMouseButtonPressed(MouseButton button);

    static void charCallback(GLFWwindow* window, unsigned int codepoint);
    static std::string getAndClearCharBuffer();
};

#endif // INPUT_HPP
