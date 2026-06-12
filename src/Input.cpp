#include "../include/Input.hpp"

GLFWwindow* Input::window = nullptr;
std::string Input::charBuffer = "";

void Input::charCallback(GLFWwindow* window, unsigned int codepoint) {
    if (codepoint < 128) {
        charBuffer += (char)codepoint;
    }
}

void Input::init(GLFWwindow* win) { 
    window = win;

    glfwSetCharCallback(win, Input::charCallback);
}

bool Input::isKeyPressed(Key key) {
    if (!window) return false;
    return glfwGetKey(window, (int)key) == GLFW_PRESS;
}

bool Input::isMouseButtonPressed(MouseButton button) {
    if (!window) return false;
    return glfwGetMouseButton(window, (int)button) == GLFW_PRESS;
}

std::string Input::getAndClearCharBuffer() {
    std::string temp = charBuffer;
    charBuffer.clear();
    return temp;
}
