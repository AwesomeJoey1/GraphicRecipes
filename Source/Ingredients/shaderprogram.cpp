#include "shaderprogram.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string WORKING_DIR = "../Source/";

namespace ShaderInfo {
    struct shaderFileExtension {
        const char *ext;
        GLSLShader::ShaderType type;
    };

    ShaderInfo::shaderFileExtension extensions[]{
            {".vs",   GLSLShader::VERTEX},
            {".vert", GLSLShader::VERTEX},
            {".fs",   GLSLShader::FRAGMENT},
            {".frag", GLSLShader::FRAGMENT},
            {".gs",   GLSLShader::GEOMETRY},
            {".geom", GLSLShader::GEOMETRY},
            {".tcs",  GLSLShader::TESS_CONTROL},
            {".tes",  GLSLShader::TESS_EVALUATION},
            {".cs",   GLSLShader::COMPUTE}

    };
}
ShaderProgram::ShaderProgram(std::string sourceDir) : _programID(0), _isLinked(false), _sourceDir(sourceDir)
{
}

ShaderProgram::~ShaderProgram()
{
    if (_programID == 0) return;


    for (auto shader : _shaders)
    {
        glDeleteShader(shader);
    }
}

void ShaderProgram::loadShader(const char *shaderPath)
{
    std::string code = loadShaderCode(shaderPath) + "\n\0";
    const GLchar * shaderCode = code.c_str();

    GLuint shader;
    GLSLShader::ShaderType shaderType = getType(shaderPath);
    shader = glCreateShader(shaderType);

    if(shader == 0)
    {
        throw ShaderException("SHADER ERROR: Could not create shader: " + std::string(shaderPath) + "\n");
    }
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    checkShaderStatus(shader);
    _shaders.push_back(shader);
}


void ShaderProgram::link()
{
    _programID = glCreateProgram();
    if (_programID == 0)
    {
        std::cout << "SHADER PROGRAMM ERROR: Could not create shader program!\n";
        exit(1);
    }

    for (auto shader : _shaders)
    {
        glAttachShader(_programID, shader);
    }

    glLinkProgram(_programID);

    checkProgramStatus(_programID);
    _isLinked = true;

    glUseProgram(_programID);

}

void ShaderProgram::use()
{

    if(_programID <= 0 || !_isLinked)
    {
        throw ShaderException("SHADER ERROR: Program has not been linked.\n");
    }
    glUseProgram(_programID);
}

/*
************************************************ PRIVATE ************************************************
*/
const std::string ShaderProgram::loadShaderCode(const char* shaderPath)
{
    std::ifstream shaderFile;
    shaderFile.open(WORKING_DIR + _sourceDir + "/" + std::string(shaderPath), std::ios::in);

    if(!shaderFile)
    {
        std::string message("SHADER ERROR: Could not open shader file: " + std::string(shaderPath) + "\n");
        throw ShaderException(message);
    }
    std::stringstream shaderCode;

    shaderCode << shaderFile.rdbuf();
    shaderFile.close();

    return shaderCode.str();
}


GLSLShader::ShaderType ShaderProgram::getType(const std::string shaderPath)
{
    size_t loc = shaderPath.find_last_of(".");
    if (loc != std::string::npos)
    {
        std::string shaderExt = shaderPath.substr(loc);
        bool extFound = false;
        for (auto extension : ShaderInfo::extensions)
        {
            if (shaderExt == extension.ext)
            {
                extFound = true;
                return extension.type;
            }
        }
        if (!extFound)
        {
            throw ShaderException("SHADER ERROR: Unknown shader extension: " + shaderExt);
        }
    }
    else {
        throw ShaderException("SHADER ERROR: Could not find shader type.\n");
    }
}

void ShaderProgram::checkShaderStatus(GLuint shaderID)
{
    GLint results;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &results);
    if (results == GL_FALSE)
    {
        GLint logLen;
        std::string logString;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);

        if(logLen > 0)
        {
            char * log = new char[logLen];
            GLsizei written;
            glGetShaderInfoLog(shaderID, logLen, &written, log);

            logString = std::string(log);
            delete [] log;
        }
        throw ShaderException("SHADER ERROR: Failed to compile shader. " + logString);
    }
}

void ShaderProgram::checkProgramStatus(GLuint programID)
{
    GLint status;
    glGetProgramiv(_programID, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cout << "SHADER PROGRAM ERROR: Failed to link shader programm\n";

        GLint logLen;
        std::string logString;
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &logLen);

        if(logLen > 0)
        {
            char * log = new char[logLen];
            GLsizei written;
            glGetProgramInfoLog(_programID, logLen, &written, log);

            logString = std::string(log);
            delete [] log;
        }

        throw ShaderException("SHADER ERROR: Could not link program: " + logString);
    }
}