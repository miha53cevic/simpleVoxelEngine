#include "application.h"
#include "glad/glad.h"
#include "../states/playing.h"

#include <iostream>

Application::Application(int width, int height, std::string title)
    : m_window(width, height, title)
{
}

Application::~Application()
{
}

void Application::run()
{
    // Add playing state
    m_stateManager.addState(std::make_unique<Playing>(this), false);

    bool running = true;
    while (running)
    {
        sf::Event e;
        while (m_window.getWindow()->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                running = false;
            if (e.type == sf::Event::Resized)
                glViewport(0, 0, e.size.width, e.size.height);
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Q)
                running = false;

            m_stateManager.CurrentState()->HandleInput(e);
        }

        // Calculate elapsed time
        sf::Time elapsed = m_window.calculateElapsedTime();
        m_window.EnableFPSCounter();

        m_stateManager.CurrentState()->Update(elapsed);

        m_window.clear();

        // Drawing
        m_stateManager.CurrentState()->Draw();

        m_window.display();
    }
}

Window * Application::getWindow()
{
    return &m_window;
}
