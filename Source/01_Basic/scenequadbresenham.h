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
    unsigned int _coordSystemRange;
    std::vector<glm::vec3> _image;


    void loadAndCompileShaders();

    void naiveLineAlgorithm(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color);
    void bresenhamLine(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color);
    void bresenhamCircle(glm::ivec2 center, unsigned int radius, glm::vec3 color);
    void drawCirclePixels(int x, int y, int cx, int cy, glm::vec3 color);
    void setPixel(int x, int y, glm::vec3 color);
    glm::ivec2 transform(glm::ivec2& fromPoint, glm::ivec2& toPoint, int& mirrorX, int& mirrorY, bool& switched);

    // predefined drawing structures
    void drawHead();
    void drawThugGlasses();
    void drawCoordinateSystem();
    void drawOctantTest();

};