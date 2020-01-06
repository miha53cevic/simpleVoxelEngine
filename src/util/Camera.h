#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera()
    {
        m_position = glm::vec3(0, 0, 0);
        m_rotation = glm::vec3(0, 0, 0);
    }

    void Update(sf::Time elapsed, sf::Window* window, float MovementSpeed = 10, float Sensetivity = 0.25f)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            // Calculate the directional vector and add it to camera position
            m_position.z -= MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
            m_position.x -= MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            m_position.z += MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
            m_position.x += MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            m_position.x += MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
            m_position.z -= MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            m_position.x -= MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
            m_position.z += MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            m_position.y -= MovementSpeed * elapsed.asSeconds();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            m_position.y += MovementSpeed * elapsed.asSeconds();
        }

        // Get the offset between mouse movements then add them to rotation
        sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
        sf::Vector2i screenCenter = sf::Vector2i(window->getSize().x / 2, window->getSize().y / 2);
        
        if (window->hasFocus())
        {
            float xOffset = mousePos.x - screenCenter.x;
            float yOffset = mousePos.y - screenCenter.y;

            xOffset *= Sensetivity;
            yOffset *= Sensetivity;

            m_rotation.x += yOffset;
            m_rotation.y += xOffset;

            // Set max up/down rotation
            if (m_rotation.x > 90.0f)
                m_rotation.x = 90.0f;

            if (m_rotation.x < -90.0f)
                m_rotation.x = -90.0f;

            // Set mouse position to the center of the screen
            if (xOffset != 0 || yOffset != 0)
                sf::Mouse::setPosition(screenCenter, *window);
        }
    }

    glm::vec3 getPosition() { return m_position; }
    glm::vec3 getRotation() { return m_rotation; }

private:
    glm::vec3 m_position;
    // Rotations are: Pitch, Yaw, Roll
    glm::vec3 m_rotation;
};
