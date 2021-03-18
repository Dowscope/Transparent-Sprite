#ifndef SCREEN_H
#define SCREEN_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

static void glfwError(int id, const char* description)
{
  std::cout << description << std::endl;
}

class Screen
{
private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    GLFWwindow * _mainWindow;
    VkInstance _instance;

    bool _initWindow();
    bool _initVulkan();
    void _createInstance();
public:
    Screen();
    ~Screen();
    bool isWindowClosed();

    bool hasInitiallized = false;
};

Screen::Screen()
{
    hasInitiallized = _initWindow();
}

Screen::~Screen()
{
    glfwDestroyWindow(_mainWindow);
    glfwTerminate();
}

bool Screen::isWindowClosed()
{
    return glfwWindowShouldClose(_mainWindow);
}

bool Screen::_initWindow()
{
    glfwSetErrorCallback(&glfwError);
    if (glfwInit() == GLFW_FALSE)
    {
        std::cout << "SCREEN_H: GLFW did not initialize";
        return false;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", glfwGetPrimaryMonitor(), NULL);

    if (!_mainWindow)
    {   
        std::cout << "SCREEN_H: Main window did not create";
        return false;
    }

    return true;
}

bool Screen::_initVulkan()
{
    _createInstance();
    return true;
}

void Screen::_createInstance()
{

}

#endif