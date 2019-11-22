#pragma once

#include "scene.h"
#include "shaderprogram.h"

class SceneQuadBresenham: public Scene
{
public:
    SceneQuadBresenham();

    void init();

    void update(float t);

    void render();

    void resize(int width, int height);

private:
    ShaderProgram* _shaderProg;
    GLuint _vao;

    void loadAndCompileShaders();

};