#pragma once

#include <glad/glad.h>
#include <stdexcept>
#include <vector>


namespace GLSLShader {
    enum ShaderType {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER,
        TESS_CONTROL = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
        COMPUTE = GL_COMPUTE_SHADER
    };
};

class ShaderException : public std::runtime_error
{
public:
    ShaderException(const std::string message) : std::runtime_error(message) {}
};


class ShaderProgram
{
public:
    ShaderProgram(std::string sourceDir);
    ~ShaderProgram();

    void loadShader(const char* shaderPath);
    void link();
    void use();

private:
    GLuint _programID;
    bool _isLinked;
    std::string _sourceDir;
    std::vector<GLuint> _shaders;

    const std::string loadShaderCode(const char* shaderPath);
    GLSLShader::ShaderType getType(const std::string shaderPath);
    void checkShaderStatus(GLuint shaderID);
    void checkProgramStatus(GLuint programID);
};
