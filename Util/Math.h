#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
};

