#ifndef ECHOGUI_HPP
#define ECHOGUI_HPP

#include <iostream>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../include/Input.hpp"

enum class WindowScaleMode {
    STRETCH,
    EXPAND,
    LETTERBOX
};

class EchoGUI {
    private:
        void init_glfw();
        
        friend void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        GLFWwindow* window;
        static unsigned int scr_width, scr_height;
        float bgColors[3];

        bool needRender = true;
    public:
        WindowScaleMode scaleMode = WindowScaleMode::STRETCH;

        EchoGUI();
        ~EchoGUI();
        int create_window(const char* winName, unsigned int w, unsigned int h);
        void run(std::function<void()> update_callback, std::function<void()> draw_callback);
        void close();

        void setBackgroundColor(float r, float g, float b);

        GLFWwindow*  getWindow() const;
        static unsigned int getWindowWidth();
        static unsigned int getWindowHeight();
        glm::vec2 getMousePosition() const;

        void requestRender();
        bool situationRender() const;
        void clearRenderReq();
};

#endif // ECHOGUI_HPP
