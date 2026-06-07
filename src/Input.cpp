#include "../include/Input.hpp"

GLFWwindow* Input::window = nullptr;

void Input::init(GLFWwindow* win) { 
    window = win;
}

bool Input::isKeyPressed(Key key) {
    if (!window) return false;
    return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(MouseButton button) {
    if (!window) return false;
    return glfwGetMouseButton(window, static_cast<int>(button)) == GLFW_PRESS;
}
