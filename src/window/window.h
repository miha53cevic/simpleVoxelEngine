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

    void clear(float r = 0.2f, float g = 0.3f, float b = 0.3f, float a = 1.0f);
    void display();

    sf::Time calculateElapsedTime();

    sf::Window* getWindow();

private:
    sf::Window m_window;
    std::string m_title;

    sf::Time m_elapsed;
    sf::Clock m_clock;
};

