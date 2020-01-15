#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/noise.hpp"

#include "Camera.h"

#include <random>

struct Math
{
public:
    Math() {}

    glm::mat4x4 static createTransformationMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
    {
        // Set identity matrix
        glm::mat4x4 matrix = glm::mat4x4(1.0f);
        matrix = glm::translate(matrix, translation);
        matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        matrix = glm::scale(matrix, scale);

        return matrix;
    }

    glm::mat4x4 static createProjectionMatrix(glm::vec2 screenSize, float FOV, float NEAR_PLANE, float FAR_PLANE)
    {
        return glm::perspective(glm::radians(FOV), (screenSize.x / screenSize.y), NEAR_PLANE, FAR_PLANE);
    }

    glm::mat4x4 static createViewMatrix(Camera* camera)
    {
        // The camera never moves the world moves opposite of the camera
        glm::mat4x4 view = glm::mat4x4(1.0f);

        view = glm::rotate(view, glm::radians(camera->getRotation().x), glm::vec3(1, 0, 0));
        view = glm::rotate(view, glm::radians(camera->getRotation().y), glm::vec3(0, 1, 0));
        view = glm::rotate(view, glm::radians(camera->getRotation().z), glm::vec3(0, 0, 1));
        view = glm::translate(view, -camera->getPosition());

        return view;
    }

    // Random number generator in given range
    float static fRandom(float first, float second)
    {
        std::random_device rd;
        std::default_random_engine generator(rd()); // rd() provides a random seed
        std::uniform_real_distribution<float> distribution(first, second);

        return distribution(generator);
    }

    int static iRandom(int first, int second)
    {
        std::random_device rd;
        std::default_random_engine generator(rd()); // rd() provides a random seed
        std::uniform_int_distribution<int> distribution(first, second);

        return distribution(generator);
    }

    // Maps values from one range to another
    float static map(float s, float a1, float a2, float b1, float b2)
    {
        return b1 + ((s - a1) * (b2 - b1)) / (a2 - a1);
    }

    // Get 2D simplex noise
    // - algorithm from: https://github.com/fogleman/Craft/blob/master/deps/noise/noise.c
    // - octaves: how many times should you combine the noise
    // - persistence: changes the height values (lower values add more height, higher values make the height deeper)
    // - lacunarity: frequency of sample points (lower values adds noise closer, whilst higher values make the noise more distant)
    float static simplex2(float x, float y, int octaves, float persistence, float lacunarity) 
    {
        float freq = 1.0f;
        float amp = 1.0f;
        float max = 1.0f;
        float total = glm::simplex(glm::vec2(x, y));
        int i;
        for (i = 1; i < octaves; i++) {
            freq *= lacunarity;
            amp *= persistence;
            max += amp;
            total += glm::simplex(glm::vec2(x * freq, y * freq)) * amp;
        }
        // Map from [-1, 1] to [0, 1]
        return (1 + total / max) / 2;
    }

    // Get 3D simplex noise
    float static simplex3(float x, float y, float z, int octaves, float persistence, float lacunarity)
    {
        float freq = 1.0f;
        float amp = 1.0f;
        float max = 1.0f;
        float total = glm::simplex(glm::vec3(x, y, z));
        int i;
        for (i = 1; i < octaves; ++i) {
            freq *= lacunarity;
            amp *= persistence;
            max += amp;
            total += glm::simplex(glm::vec3(x * freq, y * freq, z * freq)) * amp;
        }
        // Map from [-1, 1] to [0, 1]
        return (1 + total / max) / 2;
    }
};

