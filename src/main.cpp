#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include "screen.h"

Screen * screen;

void mainLoop()
{
    while (!screen->isWindowClosed())
    {
        glfwPollEvents();
    }
}

void cleanUp()
{
    delete screen;
}

int main(int argc, char const *argv[])
{
    screen = new Screen();
    if(screen->hasInitiallized == false) {return 1;}

    mainLoop();
    cleanUp();
    return 0;
}

