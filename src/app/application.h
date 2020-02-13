#pragma once
#include "window/window.h"
#include "../states/stateManager/stateManager.h"

class Application
{
public:
    Application(int width, int height, std::string title);
    ~Application();

    void run();

    Window* getWindow();

private:
    Window m_window;
    StateManager m_stateManager;
};