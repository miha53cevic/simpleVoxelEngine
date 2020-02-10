#pragma once
#include <SFML/Window.hpp>
#include <string>

class Window
{
public:
    Window(int width, int height, std::string title);
    ~Window();

    int ScreenWidth();
    int ScreenHeight();

    void EnableVSync(bool vsync);
    void EnableFPSCounter();
    void EnableWireframe(bool wireframe);

    void clear(float r = 51, float g = 76.5f, float b = 76.5f, float a = 255);
    void display();

    sf::Time calculateElapsedTime();

    sf::Window* getWindow();

private:
    sf::Window m_window;
    std::string m_title;

    sf::Time m_elapsed;
    sf::Clock m_clock;
};

