#pragma once
#include "window/window.h"

class Application
{
public:
    Application(int width, int height, std::string title);
    ~Application();

    void run();

private:
    Window m_window;
};