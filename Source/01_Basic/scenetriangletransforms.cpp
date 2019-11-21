#include "scenetriangletransforms.h"
#include "glm/gtc/matrix_transform.hpp"

SceneTriangleTransforms::SceneTriangleTransforms()
{
    _shaderProg = new ShaderProgram("01_Basic");
    _modelMatrix = glm::mat4(1.0f);
    _angle = 0.0f;
    _scale = 1.0f;
    _scaleFactor = 1;
}

void SceneTriangleTransforms::init()
{
    loadAndCompileShaders();

    _trianglePositions.emplace_back(glm::vec3(-0.5f, -0.5f, 0.0f));
    _trianglePositions.emplace_back(glm::vec3(0.5f, -0.5f, 0.0f));
    _trianglePositions.emplace_back(glm::vec3(0.0f, 0.5f, 0.0f));

    _triangleColors.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f));
    _triangleColors.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));
    _triangleColors.emplace_back(glm::vec3(0.0f, 0.0f, 1.0f));

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    GLuint vbo, cbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(glm::vec3), &_trianglePositions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);

    glBufferData(GL_ARRAY_BUFFER, 3*sizeof(glm::vec3), &_triangleColors[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void SceneTriangleTransforms::update(float t)
{
    if (_animating)
    {
        _angle += 1.0f;
        if (_angle > 360.0f)
        {
            _angle -= 360.0f;
        }

        _scale += 0.005 * _scaleFactor;
        if (_scale > 1.5f || _scale < 0.5)
        {
            _scaleFactor *= -1;
        }
    }
}

void SceneTriangleTransforms::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(_vao);
    _modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(_angle), glm::vec3(0.0f,0.0f, 1.0f));
    _modelMatrix = glm::scale(_modelMatrix, glm::vec3(_scale, _scale, 1.0f));
    _shaderProg->setUniform("ModelMatrix", _modelMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void SceneTriangleTransforms::resize(int width, int height)
{
    glViewport(0,0, width, height);
}

void SceneTriangleTransforms::loadAndCompileShaders()
{
    _shaderProg->loadShader("shader/triangletransforms.vert");
    _shaderProg->loadShader("shader/triangletransforms.frag");
    _shaderProg->link();
    _shaderProg->use();
    _shaderProg->printUniformInfo();
}
