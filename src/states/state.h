#pragma once
#include <SFML/Graphics.hpp>

class Application;

class State
{
public:
    State(Application* app) : m_app(app) {}

    virtual void init()                    = 0;
    virtual void HandleInput(sf::Event& e) = 0;
    virtual void Update(sf::Time elapsed)  = 0;
    virtual void Draw()                    = 0;

    virtual void pause()  {}
    virtual void resume() {}

protected:
    Application* m_app;
};