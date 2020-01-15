#pragma once
#include "Camera.h"
#include "../world/ChunkManager.h"
#include "../world/BlockManager.h"
#include "ToggleKey.h"

struct Player
{
    Player(Camera* cam)
        : m_camera(cam)
        , m_height(1.5f)
        , m_position(0, 128, 0)
        , m_velocity(0, 0, 0)
        , m_toggleSpaceKey(sf::Keyboard::Space)
        , m_collisions(false)
        , m_jumping(false)
    {}

    void Update(ChunkManager* manager, sf::Time elapsed, sf::Window* window, float MovementSpeed = 10, float Sensetivity = 0.25f)
    {
        glm::vec3 m_rotation = m_camera->getRotation();

        if (m_collisions)
        {
            // Update movement
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                // Calculate the directional vector and add it to camera position
                m_velocity.z -= MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
                m_velocity.x -= MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                m_velocity.z += MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
                m_velocity.x += MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                m_velocity.x += MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
                m_velocity.z -= MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                m_velocity.x -= MovementSpeed * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
                m_velocity.z += MovementSpeed * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
            }

            if (m_toggleSpaceKey.isKeyPressed() && !m_jumping)
            {
                m_jumping = true;
                m_velocity.y = MovementSpeed * elapsed.asSeconds() * m_height;
            }

            // TODO: GO for a fixed time step
            // https://gafferongames.com/post/fix_your_timestep/

            // Add gravity
            m_velocity.y -= 1 * elapsed.asSeconds();

            // Check for collision
            glm::vec3 movedPos = m_position + m_velocity;
            int w = manager->getChunksSize().x;
            int h = manager->getChunksSize().y;
            int d = manager->getChunksSize().z;

            int x = static_cast<int>(movedPos.x);
            int y = static_cast<int>(movedPos.y);
            int z = static_cast<int>(movedPos.z);

            // Add extra offset to movedPos because the camera clips the faces otherwise
            // check if we need to add + or - offset depending on the direction we are moving to so (newPos - curPos)
            if (movedPos.x - m_position.x < 0)
                movedPos.x -= 0.2f;
            else
                movedPos.x += 0.2f;

            if (movedPos.z - m_position.z < 0)
                movedPos.z -= 0.2f;
            else
                movedPos.z += 0.2f;


            if (x >= 0 && y >= 0 && z >= 0 && x < Chunk::Width * w && y < Chunk::Height * h && z < Chunk::Depth * d)
            {
                // NOTE: The player is 2 blocks in height so we have to check 2 blocks for x, y, z

                // X-Axis
                // Check upper (camera) block
                if (manager->getBlock(movedPos.x, m_position.y, m_position.z) != BLOCK::AIR)
                    m_velocity.x = 0;
                // Check lower block
                if (manager->getBlock(movedPos.x, m_position.y - m_height, m_position.z) != BLOCK::AIR)
                    m_velocity.x = 0;

                // Y-Axis
                // Same thing, check the block under the player and above the player
                if (manager->getBlock(m_position.x, movedPos.y - m_height, m_position.z) != BLOCK::AIR)
                {
                    m_velocity.y = 0; 

                    // When player hits the ground enable jumping again
                    m_jumping = false;
                }
                if (manager->getBlock(m_position.x, movedPos.y, m_position.z) != BLOCK::AIR)
                    m_velocity.y = 0;

                // Z-Axis
                if (manager->getBlock(m_position.x, m_position.y, movedPos.z) != BLOCK::AIR)
                    m_velocity.z = 0;
                if (manager->getBlock(m_position.x, m_position.y - m_height, movedPos.z) != BLOCK::AIR)
                    m_velocity.z = 0;
            }

            // Update the position
            m_position.x += m_velocity.x;
            m_position.y += m_velocity.y;
            m_position.z += m_velocity.z;

            // clear velocity after each update
            m_velocity.x = 0;
            m_velocity.z = 0;
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                // Calculate the directional vector and add it to camera position
                m_position.z -= 25 * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
                m_position.x -= 25 * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                m_position.z += 25 * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
                m_position.x += 25 * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                m_position.x += 25 * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
                m_position.z -= 25 * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                m_position.x -= 25 * elapsed.asSeconds() * cosf(glm::radians(-m_rotation.y));
                m_position.z += 25 * elapsed.asSeconds() * sinf(glm::radians(-m_rotation.y));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            {
                m_position.y -= 25 * elapsed.asSeconds();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                m_position.y += 25 * elapsed.asSeconds();
            }
        }

        // Update the camera
        m_camera->Update(elapsed, window, MovementSpeed, Sensetivity);
        m_camera->updatePosition(m_position);
    }

    Camera*   m_camera;

    glm::vec3 m_position;
    glm::vec3 m_velocity;
    float     m_height;

    bool m_collisions;
    bool m_jumping;

    ToggleKey m_toggleSpaceKey;
};
