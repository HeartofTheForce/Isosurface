#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
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
