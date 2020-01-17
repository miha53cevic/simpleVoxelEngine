#include "window/window.h"
#include "util/Ray.h"
#include "util/Cube.h"
#include "util/Player.h"

#include "world/ChunkManager.h"

#include <iostream>

int main()
{
    Window window(1280, 720, "Woxel");

    gl::Shader shader;
    shader.setAttribute(0, "position");
    shader.setAttribute(1, "textureCoords");
    shader.setAttribute(2, "normals");
    
    shader.createProgram("deps/glsl/shader1");

    shader.setUniformLocation("transformationMatrix");
    shader.setUniformLocation("projectionMatrix");
    shader.setUniformLocation("viewMatrix");

    gl::Shader simple_shader;
    simple_shader.setAttribute(0, "position");

    simple_shader.createProgram("deps/glsl/simple_colour");

    simple_shader.setUniformLocation("transformationMatrix");
    simple_shader.setUniformLocation("projectionMatrix");
    simple_shader.setUniformLocation("viewMatrix");

    gl::TextureAtlas atlas("deps/Resources/textures/textureAtlas.png", 2048, 256);

    glm::mat4 projectionMatrix = Math::createProjectionMatrix(glm::vec2(window.ScreenWidth(), window.ScreenHeight()), 90, 0.1, 1000);

    shader.Bind();
    shader.loadMatrix(shader.getUniformLocation("projectionMatrix"), projectionMatrix);
    shader.Unbind();

    simple_shader.Bind();
    simple_shader.loadMatrix(simple_shader.getUniformLocation("projectionMatrix"), projectionMatrix);
    simple_shader.Unbind();

    Camera camera;
    Player player(&camera);
    player.m_collisions = true;

    std::uint8_t selectedBlock = 1;

    // Used when the player presses left mouse click
    glm::vec3 rayLastPosition;

    // Used for block placement
    glm::vec3 rayUnitVector;

    // Displays a border of white lines around the current looking block
    Entity selectedBlockBox;

    ChunkManager chunk_manager;
    chunk_manager.generate(4, 1, 4, &atlas);

    // Delete faces in the inside of the mesh
    // Vertecies must be Counter-Clock Wise
    glEnable(GL_CULL_FACE);
        
    bool wireframe = false;
    bool running = true;

    window.EnableVSync(true);

    // TODO: Render text

    while (running)
    {
        sf::Event e;
        while (window.getWindow()->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
                running = false;
            if (e.type == sf::Event::Resized)
                glViewport(0, 0, e.size.width, e.size.height);
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Q)
                running = false;

            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Tab)
            {
                wireframe = !wireframe;
                if (wireframe) window.EnableWireframe(true);
                else           window.EnableWireframe(false);
            }

            // Temporary block changing method
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::B)
            {
                if (selectedBlock == 5) selectedBlock = 1;
                else selectedBlock++;
            }

            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
            {
                // Raycasting
                int x = rayLastPosition.x;
                int y = rayLastPosition.y;
                int z = rayLastPosition.z;

                if (chunk_manager.getBlock(x, y, z))
                {
                    // Remove block and update the chunk
                    chunk_manager.setBlock(x, y, z, 0);
                    chunk_manager.getChunkFromWorldPosition(x, y, z)->Update();
                }
            }

            if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Right)
            {
                glm::vec3 temp_ray = rayLastPosition;

                // Cast a ray going backwards until we hit an AIR block
                bool outOfBounds = false;
                do
                {
                    temp_ray.x += rayUnitVector.x * 0.05f;
                    temp_ray.y += rayUnitVector.y * 0.05f;
                    temp_ray.z += rayUnitVector.z * 0.05f;

                    // Check if the ray is still in bounds
                    int w = chunk_manager.getChunksSize().x;
                    int h = chunk_manager.getChunksSize().y;
                    int d = chunk_manager.getChunksSize().z;

                    if (temp_ray.x >= 0 && temp_ray.y >= 0 && temp_ray.z >= 0 && temp_ray.x < Chunk::Width * w && temp_ray.y < Chunk::Height * h && temp_ray.z < Chunk::Depth * d)
                        continue;
                    else
                    {
                        outOfBounds = true;
                        break;
                    }

                } while (chunk_manager.getBlock(temp_ray.x, temp_ray.y, temp_ray.z));
                
                if (!outOfBounds)
                {
                    int x = temp_ray.x;
                    int y = temp_ray.y;
                    int z = temp_ray.z;

                    // Check if the block can be placed based on the players position
                    if (x != (int)player.m_position.x || (y != (int)player.m_position.y && y != (int)(player.m_position.y - 0.25f)) || z != (int)player.m_position.z)
                    {
                        // Place block and update the chunk
                        chunk_manager.setBlock(x, y, z, selectedBlock);
                        chunk_manager.getChunkFromWorldPosition(x, y, z)->Update();
                    }
                }
            }
        }

        //////////////////////////////
        //        RayTracing        //
        /////////////////////////////
        for (Ray ray(camera.getPosition(), camera.getRotation()); ray.getLength() < 6; ray.step(0.05f))
        {
            glm::vec3 position = ray.getEnd();
            //printf("%f, %f, %f\n\n", position.x, position.y, position.z);

            int x = static_cast<int>(position.x);
            int y = static_cast<int>(position.y);
            int z = static_cast<int>(position.z);

            // Don't check the ray if we are outside of the chunks
            int w = chunk_manager.getChunksSize().x;
            int h = chunk_manager.getChunksSize().y;
            int d = chunk_manager.getChunksSize().z;

            if (x >= 0 && y >= 0 && z >= 0 && x < Chunk::Width * w && y < Chunk::Height * h && z < Chunk::Depth * d)
            {
                if (chunk_manager.getBlock(x, y, z))
                {
                    // Save the last ray position and unitVector
                    rayLastPosition = position;
                    
                    rayUnitVector.x = glm::cos(glm::radians(camera.getRotation().y + 90));
                    rayUnitVector.z = glm::sin(glm::radians(camera.getRotation().y + 90));
                    rayUnitVector.y = glm::tan(glm::radians(camera.getRotation().x));
                    
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
                    
                    if (selectedBlockBox.VBOs.empty()) selectedBlockBox.setVBO(linePoints, 0, 3);
                    else                               selectedBlockBox.VBOs[0]->setData(linePoints, 0, 3);

                    break;
                }
            }
        }

        sf::Time elapsed = window.calculateElapsedTime();
        window.EnableFPSCounter();

        window.clear();

        player.Update(&chunk_manager, elapsed, window.getWindow(), 7.5f);

        // ...Drawing
        auto Draw = [&](Entity* entity, gl::Shader* shader, gl::Texture* texture)
        {
            shader->Bind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture->texture);

            shader->loadMatrix(shader->getUniformLocation("viewMatrix"), Math::createViewMatrix(&camera));
            entity->VAO.Bind();

            shader->loadMatrix(shader->getUniformLocation("transformationMatrix"), Math::createTransformationMatrix(entity->position, entity->rotation, entity->scale));

            glDrawElements(GL_TRIANGLES, entity->EBO.size, GL_UNSIGNED_INT, 0);
            entity->VAO.Unbind();

            shader->Unbind();
        };

        // Render the chunk
        chunk_manager.render(&shader, &atlas.texture, &camera);

        // Draw the border box of current voxel
        simple_shader.Bind();
        simple_shader.loadMatrix(simple_shader.getUniformLocation("viewMatrix"), Math::createViewMatrix(&camera));

        selectedBlockBox.VAO.Bind();

        simple_shader.loadMatrix(simple_shader.getUniformLocation("transformationMatrix"), Math::createTransformationMatrix({0, 0, 0}, { 0, 0, 0 }, { 1, 1, 1 }));

        glLineWidth(5);
        glDrawArrays(GL_LINES, 0, 72);

        selectedBlockBox.VAO.Unbind();
        simple_shader.Unbind();
        
        window.display();
    }

	return 0;
}