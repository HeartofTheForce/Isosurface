#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputHandler
{
    GLFWwindow* _window;

    void UpdateMouse();
    bool _mouseMoved = false;
    double _mouseX{0};
    double _mouseY{0};
    double _mouseDeltaX{0};
    double _mouseDeltaY{0};

  public:
    float CurrentTime{0};
    float DeltaTime{0};
    bool BeginFrame();
    void EndFrame();

    glm::vec3 GetMoveDirection();
    glm::vec2 GetMouseDelta();

    InputHandler(GLFWwindow* window)
        : _window(window)
    {
    }
};
