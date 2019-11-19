#pragma once
#include "scene.h"
#include "shaderprogram.h"

class SceneTriangleTransforms : public Scene {
public:
    SceneTriangleTransforms();

    void init();

    void update(float t);

    void render();

    void resize(int width, int height);

private:
    ShaderProgram* _shaderProg;
    GLuint _vao;

    void loadAndCompileShaders();

};