#include <Utility/InputHandler.h>

void InputHandler::UpdateMouse()
{
    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);

    if (!_mouseMoved)
    {
        _mouseX = xpos;
        _mouseY = ypos;
        _mouseMoved = true;
    }

    _mouseDeltaX = xpos - _mouseX;
    _mouseDeltaY = ypos - _mouseY;

    _mouseX = xpos;
    _mouseY = ypos;
}

bool InputHandler::BeginFrame()
{
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(_window, GLFW_TRUE);

    if (glfwWindowShouldClose(_window))
        return false;

    glfwPollEvents();
    UpdateMouse();

    float newTime = glfwGetTime();
    DeltaTime = newTime - CurrentTime;
    CurrentTime = newTime;

    return true;
}

void InputHandler::EndFrame()
{
    glfwSwapBuffers(_window);
}

glm::vec3 InputHandler::GetMoveDirection()
{
    glm::vec3 moveDirection = glm::vec3(0.0f);
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        moveDirection.x += 1.0f;
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        moveDirection.x -= 1.0f;
    if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS)
        moveDirection.y += 1.0f;
    if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        moveDirection.y -= 1.0f;
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        moveDirection.z += 1.0f;
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        moveDirection.z -= 1.0f;

    return moveDirection;
}

glm::vec2 InputHandler::GetMouseDelta()
{
    return glm::vec2(_mouseDeltaX, _mouseDeltaY);
}
