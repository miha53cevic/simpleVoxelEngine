#include "playing.h"

#include "../app/application.h"

Playing::Playing(Application * app)
    : State(app)
    , m_atlas("deps/Resources/textures/textureAtlas.png", 2048, 256)
    , m_player(&m_camera)
    , m_selectedBlock(1)
    , m_wireframe(false)
{
}

void Playing::init()
{
    // Delete faces in the inside of the mesh
    // Vertecies must be Counter-Clock Wise
    glEnable(GL_CULL_FACE);

    // Main shader
    m_shader.setAttribute(0, "position");
    m_shader.setAttribute(1, "textureCoords");
    m_shader.setAttribute(2, "normals");

    m_shader.createProgram("deps/glsl/shader1");

    m_shader.setUniformLocation("transformationMatrix");
    m_shader.setUniformLocation("projectionMatrix");
    m_shader.setUniformLocation("viewMatrix");

    // Shader for creating the selected block outline
    m_simple_shader.setAttribute(0, "position");

    m_simple_shader.createProgram("deps/glsl/simple_colour");

    m_simple_shader.setUniformLocation("transformationMatrix");
    m_simple_shader.setUniformLocation("projectionMatrix");
    m_simple_shader.setUniformLocation("viewMatrix");

    // Create the projection matrix
    m_projectionMatrix = Math::createProjectionMatrix(glm::vec2(m_app->getWindow()->ScreenWidth(), m_app->getWindow()->ScreenHeight()), 90, 0.1, 1000);

    // Load the projection matrix to both shaders
    m_shader.Bind();
    m_shader.loadMatrix(m_shader.getUniformLocation("projectionMatrix"), m_projectionMatrix);
    m_shader.Unbind();

    m_simple_shader.Bind();
    m_simple_shader.loadMatrix(m_simple_shader.getUniformLocation("projectionMatrix"), m_projectionMatrix);
    m_simple_shader.Unbind();

    // Enable or disable player collisions
    m_player.m_collisions = true;

    // Generate the chunks
    m_chunk_manager.generate(1, 1, 1, &m_atlas);

    // Enable V-Sync
    m_app->getWindow()->EnableVSync(true);
}

void Playing::HandleInput(sf::Event & e)
{
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab)
    {
        m_wireframe = !m_wireframe;
        if (m_wireframe) m_app->getWindow()->EnableWireframe(true);
        else             m_app->getWindow()->EnableWireframe(false);
    }

    // Temporary block changing method
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::B)
    {
        if (m_selectedBlock == 6) m_selectedBlock = 1;
        else m_selectedBlock++;
    }

    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
    {
        // Raycasting
        int x = m_rayLastPosition.x;
        int y = m_rayLastPosition.y;
        int z = m_rayLastPosition.z;

        if (m_chunk_manager.getBlock(x, y, z))
        {
            // Remove block and update the chunk
            m_chunk_manager.setBlock(x, y, z, 0);
            m_chunk_manager.getChunkFromWorldPosition(x, y, z)->Update();
        }
    }

    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Right)
    {
        glm::vec3 temp_ray = m_rayLastPosition;

        // Cast a ray going backwards until we hit an AIR block
        bool outOfBounds = false;
        do
        {
            temp_ray.x += m_rayUnitVector.x * 0.05f;
            temp_ray.y += m_rayUnitVector.y * 0.05f;
            temp_ray.z += m_rayUnitVector.z * 0.05f;

            // Check if the ray is still in bounds
            int w = m_chunk_manager.getChunksSize().x;
            int h = m_chunk_manager.getChunksSize().y;
            int d = m_chunk_manager.getChunksSize().z;

            if (temp_ray.x >= 0 && temp_ray.y >= 0 && temp_ray.z >= 0 && temp_ray.x < Chunk::Width * w && temp_ray.y < Chunk::Height * h && temp_ray.z < Chunk::Depth * d)
                continue;
            else
            {
                outOfBounds = true;
                break;
            }

        } while (m_chunk_manager.getBlock(temp_ray.x, temp_ray.y, temp_ray.z));

        if (!outOfBounds)
        {
            int x = temp_ray.x;
            int y = temp_ray.y;
            int z = temp_ray.z;

            // Check if the block can be placed based on the players position
            if (x != (int)m_player.m_position.x || (y != (int)m_player.m_position.y && y != (int)(m_player.m_position.y - 0.25f)) || z != (int)m_player.m_position.z)
            {
                // Place block and update the chunk
                m_chunk_manager.setBlock(x, y, z, m_selectedBlock);
                m_chunk_manager.getChunkFromWorldPosition(x, y, z)->Update();
            }
        }
    }
}

void Playing::Update(sf::Time elapsed)
{
    //////////////////////////////
    //        RayTracing        //
    /////////////////////////////
    for (Ray ray(m_camera.getPosition(), m_camera.getRotation()); ray.getLength() < 6; ray.step(0.05f))
    {
        glm::vec3 position = ray.getEnd();
        //printf("%f, %f, %f\n\n", position.x, position.y, position.z);

        int x = static_cast<int>(position.x);
        int y = static_cast<int>(position.y);
        int z = static_cast<int>(position.z);

        // Don't check the ray if we are outside of the chunks
        int w = m_chunk_manager.getChunksSize().x;
        int h = m_chunk_manager.getChunksSize().y;
        int d = m_chunk_manager.getChunksSize().z;

        if (x >= 0 && y >= 0 && z >= 0 && x < Chunk::Width * w && y < Chunk::Height * h && z < Chunk::Depth * d)
        {
            if (m_chunk_manager.getBlock(x, y, z))
            {
                // Save the last ray position and unitVector
                m_rayLastPosition = position;

                m_rayUnitVector.x = glm::cos(glm::radians(m_camera.getRotation().y + 90));
                m_rayUnitVector.z = glm::sin(glm::radians(m_camera.getRotation().y + 90));
                m_rayUnitVector.y = glm::tan(glm::radians(m_camera.getRotation().x));

                // Create block box border
                float bx = x;
                float by = y;
                float bz = z;

                std::vector<GLfloat> linePoints = {
                    bx + 0, by + 0, bz + 0,
                    bx + 1, by + 0, bz + 0,
                    bx + 0, by + 1, bz + 0,
                    bx + 1, by + 1, bz + 0,
                    bx + 0, by + 0, bz + 1,
                    bx + 1, by + 0, bz + 1,
                    bx + 0, by + 1, bz + 1,
                    bx + 1, by + 1, bz + 1,

                    bx + 0, by + 0, bz + 0,
                    bx + 0, by + 1, bz + 0,
                    bx + 1, by + 0, bz + 0,
                    bx + 1, by + 1, bz + 0,
                    bx + 0, by + 0, bz + 1,
                    bx + 0, by + 1, bz + 1,
                    bx + 1, by + 0, bz + 1,
                    bx + 1, by + 1, bz + 1,

                    bx + 0, by + 0, bz + 0,
                    bx + 0, by + 0, bz + 1,
                    bx + 1, by + 0, bz + 0,
                    bx + 1, by + 0, bz + 1,
                    bx + 0, by + 1, bz + 0,
                    bx + 0, by + 1, bz + 1,
                    bx + 1, by + 1, bz + 0,
                    bx + 1, by + 1, bz + 1,

                };

                if (m_selectedBlockBox.VBOs.empty()) m_selectedBlockBox.setVBO(linePoints, 0, 3);
                else                                 m_selectedBlockBox.VBOs[0]->setData(linePoints, 0, 3);

                break;
            }
        }
    }

    m_player.Update(&m_chunk_manager, elapsed, m_app->getWindow()->getWindow(), 7.5f);
}

void Playing::Draw()
{
    // Render the chunk
    m_chunk_manager.render(&m_shader, &m_atlas.texture, &m_camera);

    // Draw the border box of current voxel
    m_simple_shader.Bind();
    m_simple_shader.loadMatrix(m_simple_shader.getUniformLocation("viewMatrix"), Math::createViewMatrix(&m_camera));

    m_selectedBlockBox.VAO.Bind();

    m_simple_shader.loadMatrix(m_simple_shader.getUniformLocation("transformationMatrix"), Math::createTransformationMatrix({ 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1 }));

    glLineWidth(5);
    glDrawArrays(GL_LINES, 0, 72);

    m_selectedBlockBox.VAO.Unbind();
    m_simple_shader.Unbind();
}

void Playing::pause()
{
}

void Playing::resume()
{
}
