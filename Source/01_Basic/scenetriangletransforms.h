#pragma once
#include "scene.h"
#include "shaderprogram.h"

#include <glm/glm.hpp>

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

    std::vector<glm::vec3> _trianglePositions;
    std::vector<glm::vec3> _triangleColors;

    float _angle;
    float _scale;
    int _scaleFactor;
    glm::mat4 _modelMatrix;

    void loadAndCompileShaders();

};