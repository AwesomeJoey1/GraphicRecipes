#pragma once

#include "scene.h"
#include "shaderprogram.h"

class SceneTriangle : public Scene
{
public:

    SceneTriangle();

    void init();

    void update(float t);

    void render();

    void resize(int width, int height);

private:
    ShaderProgram* _shaderProg;
    GLuint _vao;

    void compileAndLinkShaders();
};