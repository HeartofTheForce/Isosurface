#pragma once
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <iostream>
#include <string>

class WindowHandler
{
    WindowHandler();
    ~WindowHandler();

  public:
    GLFWwindow* CreateWindow(const int& width, const int& height, const bool& fullscreen, const std::string& title);
    static WindowHandler& Instance()
    {
        static WindowHandler instance;
        return instance;
    }

    WindowHandler(WindowHandler const&) = delete;
    WindowHandler& operator=(const WindowHandler&) = delete;
};
