#include "scenequad.h"

SceneQuad::SceneQuad()
{
    _shaderProg = new ShaderProgram("01_Basic");
}

void SceneQuad::init() {
    loadAndCompileShaders();

    const float quad[] = { -1.0f, -1.0f, 0.0f,
                           1.0f, 1.0f, 0.0f,
                           -1.0f, 1.0f, 0.0f,
                           1.0f, -1.0f, 0.0f};

    const float color[] = { 1.0f, 0.0f, 0.0f,
                            0.0f, 1.0f, 0.0f,
                            0.0f, 0.0f, 1.0f,
                            1.0f, 1.0f, 0.0f};

    const short indices[] = { 0, 1, 2,
                            0, 3, 1 };


    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    GLuint vbo, cbo, ebo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &cbo);
    glBindBuffer(GL_ARRAY_BUFFER, cbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SceneQuad::update(float t) {

}

void SceneQuad::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void SceneQuad::resize(int width, int height)
{
    glViewport(0,0,width,height);
}

void SceneQuad::loadAndCompileShaders()
{
    _shaderProg->loadShader("shader/quad.vert");
    _shaderProg->loadShader("shader/quad.frag");
    _shaderProg->link();
    _shaderProg->use();
}
