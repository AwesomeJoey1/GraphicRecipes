#include <glad/glad.h>
#include "scenetriangle.h"

SceneTriangle::SceneTriangle()
{
    _shaderProg = new ShaderProgram("01_Basic");
}

void SceneTriangle::init()
{
    // compile shaders etc.
    compileAndLinkShaders();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    const GLfloat triangleVertices[] = {-1.0f, -1.0f, 0.0f,
                                        1.0f, -1.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f};
    const GLfloat triangleColor[] = {1.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f};

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    GLuint vbo, cbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleColor), triangleColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void SceneTriangle::update(float t)
{

}

void SceneTriangle::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void SceneTriangle::resize(int width, int height)
{
    glViewport(0,0, width, height);
}

void SceneTriangle::compileAndLinkShaders()
{
    _shaderProg->loadShader("shader/triangle.vert");
    _shaderProg->loadShader("shader/triangle.frag");
    _shaderProg->link();
    _shaderProg->use();
}