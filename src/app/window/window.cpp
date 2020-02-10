#include "window.h"

#include <glad/glad.h>
#include <stdio.h>

Window::Window(int width, int height, std::string title)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;    // Add this to make the depth buffer work
    settings.stencilBits = 8;
    //settings.antialiasingLevel = 4;

    m_window.create(sf::VideoMode(width, height), title, sf::Style::Default, settings);

    m_title = title;

    // Load after window
    if (!gladLoadGL())
    {
        printf("Failed to gladLoadGL!");
        exit(-1);
    }
    else printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
}

Window::~Window()
{
    m_window.close();
}

int Window::ScreenWidth()
{
    return m_window.getSize().x;
}

int Window::ScreenHeight()
{
    return m_window.getSize().y;
}

void Window::EnableVSync(bool vsync)
{
    m_window.setVerticalSyncEnabled(vsync);
}

void Window::EnableFPSCounter()
{
    m_window.setTitle(m_title + " - FPS: " + std::to_string(1.0f / m_elapsed.asSeconds()));
}

void Window::EnableWireframe(bool wireframe)
{
    if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else           glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Window::clear(float r, float g, float b, float a)
{
    r /= 255;
    g /= 255;
    b /= 255;
    a /= 255;

    // Enable the depth buffer
    glEnable(GL_DEPTH_TEST);

    // CLEAR DEPTH BUFFER BIT so triangles don't overlap
    // CLEAR COLOR BUFFER for background screen
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::display()
{
    m_window.display();
}

sf::Time Window::calculateElapsedTime()
{
    m_elapsed = m_clock.restart();
    return m_elapsed;
}

sf::Window * Window::getWindow()
{
    return &m_window;
}
