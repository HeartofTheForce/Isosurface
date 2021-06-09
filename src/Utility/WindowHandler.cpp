#include <Utility/WindowHandler.h>

namespace
{
void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height)
{
    GLFWwindow* currentWindow = glfwGetCurrentContext();
    if (currentWindow != window)
    {
        glfwMakeContextCurrent(window);
        glViewport(0, 0, width, height);
        glfwMakeContextCurrent(currentWindow);
    }
    else
    {
        glViewport(0, 0, width, height);
    }
}
} // namespace

WindowHandler::WindowHandler()
{
    if (glfwInit() != GLFW_TRUE)
    {
        std::cout << "GLFW initialization failed!" << std::endl;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

WindowHandler::~WindowHandler()
{
    glfwTerminate();
}

GLFWwindow* WindowHandler::CreateWindow(const int& width, const int& height, const bool& fullscreen, const std::string& title)
{
    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "GLFW window creation failed!" << std::endl;
        glfwTerminate();
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
    }

    std::cout << "GL_VERSION: "
              << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL_SHADING_LANGUAGE_VERSION: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSwapInterval(0);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (mode)
    {
        int x = (mode->width - width) / 2;
        int y = (mode->height - height) / 2;
        glfwSetWindowPos(window, x, y);
    }

    return window;
}
