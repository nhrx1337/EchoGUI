#ifndef INPUT_HPP
#define INPUT_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class Key {
    ESCAPE = GLFW_KEY_ESCAPE,
    SPACE  = GLFW_KEY_SPACE,
    ENTER  = GLFW_KEY_ENTER
};

enum class MouseButton {
    LEFT   = GLFW_MOUSE_BUTTON_LEFT,
    RIGHT  = GLFW_MOUSE_BUTTON_RIGHT,
    MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
};

class Input {
private:
    static GLFWwindow* window;
    
public:
    static void init(GLFWwindow* win);

    static bool isKeyPressed(Key key);

    static bool isMouseButtonPressed(MouseButton button);
};

#endif // INPUT_HPP
