#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <map>

namespace gl
{
    class Shader
    {
    public:
        Shader();
        ~Shader();

        void createProgram(const std::string& fileName);

        void Bind();
        void Unbind();

        void setAttribute(int attributeID, std::string var_name);
        void setUniformLocation(std::string uniform_name);

        int getUniformLocation(std::string uniform_name);

        void loadFloat(int location, float value);
        void loadVector3(int location, glm::vec3 vector);
        void loadVector4(int location, glm::vec4 vector);
        void loadBool(int location, bool value);
        void loadMatrix(int location, const glm::mat4x4& matrix);

    private:
        static const unsigned int NUM_SHADERS = 2;

        std::vector<std::pair<int, std::string>> m_attributes;
        std::map<std::string, int> m_uniformLocations;

        GLuint CreateShader(const std::string& text, unsigned int type);
        std::string LoadShader(const std::string& fileName);

        GLuint m_program;
        GLuint m_shaders[NUM_SHADERS];
    };
};

