#include "../include/EchoGUI.hpp"

// Default window size coordinates
unsigned int EchoGUI::scr_width  = 800;
unsigned int EchoGUI::scr_height = 600;

void EchoGUI::init_glfw() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        std::exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

EchoGUI::EchoGUI() : window(nullptr) {
    bgColors[0] = 0.1f; bgColors[1] = 0.1f; bgColors[2] = 0.1f;
    init_glfw();
}

EchoGUI::~EchoGUI() {
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    EchoGUI* gui = (EchoGUI*)glfwGetWindowUserPointer(window);
    if (gui) gui->requestRender();

    // Handle viewport transformation based on scaling configurations
    switch (gui->scaleMode) {
        case WindowScaleMode::STRETCH:
            glViewport(0, 0, width, height);
            break;
        case WindowScaleMode::EXPAND:
            glViewport(0, 0, width, height);
            EchoGUI::scr_width = width;
            EchoGUI::scr_height = height;
            break;
        case WindowScaleMode::LETTERBOX:
            float targetAspectRatio = 800.0f / 600.0f;
            
            int vpWidth = width;
            int vpHeight = (int)(width / targetAspectRatio);

            if (vpHeight > height) {
                vpHeight = height;
                vpWidth = (int)(height * targetAspectRatio);
            }
            
            int xOffset = (width - vpWidth) / 2;
            int yOffset = (height - vpHeight) / 2;

            glViewport(xOffset, yOffset, vpWidth, vpHeight);
            break;
    }
}

int EchoGUI::create_window(const char* winName, unsigned int w, unsigned int h) {
    scr_width =  w;
    scr_height = h;
    window = glfwCreateWindow(scr_width, scr_height, winName, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    Input::init(window);

    glfwSwapInterval(1); // Enable V-Sync
    return 0;
}

void EchoGUI::run(std::function<void()> update_callback, 
                std::function<void()> draw_callback) {
    while (!glfwWindowShouldClose(window)) {
        glfwWaitEvents(); // Event-driven loop: blocks until input/window event occurs
        update_callback();

        // Conditional rendering to optimize CPU/GPU usage
        if (needRender) {
            glClearColor(bgColors[0], bgColors[1], bgColors[2], 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            draw_callback();

            glfwSwapBuffers(window);

            clearRenderReq();
        }
    }
}

void EchoGUI::close() {
    if (window) {
        glfwSetWindowShouldClose(window, true);
    }
}

void EchoGUI::setBackgroundColor(float r, float g, float b) {
    bgColors[0] = r; bgColors[1] = g; bgColors[2] = b;
}

GLFWwindow*  EchoGUI::getWindow()  const { return window;     }
unsigned int EchoGUI::getWindowWidth()   { return scr_width;  }
unsigned int EchoGUI::getWindowHeight()  { return scr_height; }

glm::vec2 EchoGUI::getMousePosition() const {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glm::vec2 rawMouse((float)xpos, (float)ypos);

    int winWidth, winHeight;
    glfwGetWindowSize(window, &winWidth, &winHeight);

    if (winWidth <= 0 || winHeight <= 0) return rawMouse;

    const float base_width = 800.0f;
    const float base_height = 600.0f;

    // Convert raw cursor coordinates back to virtual UI space coordinates
    switch (scaleMode) {
        case WindowScaleMode::STRETCH: {
            float xRatio = base_width / winWidth;
            float yRatio = base_height / winHeight;
            return glm::vec2(rawMouse.x * xRatio, rawMouse.y * yRatio);
        }
        case WindowScaleMode::EXPAND: {
            return rawMouse;
        }
        case WindowScaleMode::LETTERBOX: {
            float targetAspectRatio = base_width / base_height;

            int vpWidth = winWidth;
            int vpHeight = (int)(winWidth / targetAspectRatio);

            if (vpHeight > winHeight) {
                vpHeight = winHeight;
                vpWidth = (int)(winHeight * targetAspectRatio);
            }

            float xOffset = (winWidth - vpWidth) / 2.0f;
            float yOffset = (winHeight - vpHeight) / 2.0f;

            float correctedX = rawMouse.x - xOffset;
            float correctedY = rawMouse.y - yOffset;

            correctedX = (correctedX / vpWidth) * base_width;
            correctedY = (correctedY / vpHeight) * base_height;

            return glm::vec2(correctedX, correctedY);
        }
    }
    return rawMouse;
}


void EchoGUI::requestRender() { needRender = true; }
bool EchoGUI::situationRender() const { return needRender; }
void EchoGUI::clearRenderReq() { needRender = false; }
