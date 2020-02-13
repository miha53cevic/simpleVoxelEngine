#pragma once
#include "state.h"

#include "../util/math/Ray.h"
#include "../util/Cube.h"
#include "../player/Player.h"

#include "../world/ChunkManager.h"

class Application;

class Playing : public State
{
public:
    Playing(Application* app);

    virtual void init() override;
    virtual void HandleInput(sf::Event& e) override;
    virtual void Update(sf::Time elapsed) override;
    virtual void Draw() override;

    virtual void pause() override;
    virtual void resume() override;

private:
    gl::Shader m_shader;
    gl::Shader m_simple_shader;
    gl::TextureAtlas m_atlas;

    glm::mat4 m_projectionMatrix;

    Camera m_camera;
    Player m_player;

    std::uint8_t m_selectedBlock;

    // Used when the player presses left mouse click
    glm::vec3 m_rayLastPosition;

    // Used for block placement
    glm::vec3 m_rayUnitVector;

    // Displays a border of white lines around the current looking block
    Entity m_selectedBlockBox;

    ChunkManager m_chunk_manager;

    bool m_wireframe;
};