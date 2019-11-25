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
    unsigned int _imgSize;
    std::vector<glm::vec3> _image;


    void loadAndCompileShaders();

    void naiveLineAlgorithm(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color);
    void bresenhamLine(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color);
    void setPixel(unsigned int x, unsigned y, glm::vec3 color);

};