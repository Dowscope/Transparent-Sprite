
#include "Screen.h"

bool init();
void cleanup();
void mainLoop();

Screen* screen;


int main(int argc, char const *argv[])
{
    if(!init()) return 1;
    mainLoop();
    cleanup();
    return 0;
}

bool init()
{
    screen = new Screen();
    if (screen->hasInitialized == false)
    {
        std::cout << "MAIN.CPP: Screen did not initialize" << std::endl;
        return false;
    }
    return true;
}

void cleanup()
{
    delete screen;
}

void mainLoop()
{
    while(screen->shouldWindowClose() == false)
    {

    }
}

