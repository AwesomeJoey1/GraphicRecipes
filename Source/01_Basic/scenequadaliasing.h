#pragma once

#include "scene.h"
#include "shaderprogram.h"

class SceneQuadAliasing : public Scene
{
public:
    SceneQuadAliasing();

    void init();

    void update(float t);

    void render();

    void resize(int width, int height);

private:
    ShaderProgram* _shaderProg;
    GLuint _vao;
    unsigned int _imgSize;
    unsigned int _samplingScale;
    std::vector<glm::vec3> _image, _superSampledImage;
    std::vector<float> _filter3x3, _filter5x5, _filter7x7;


    void loadAndCompileShaders();

    void downsample();

    void bresenhamLine(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color);
    void bresenhamCircle(glm::ivec2 center, unsigned int radius, glm::vec3 color);
    void drawCirclePixels(int x, int y, int cx, int cy, const glm::vec3& color);
    void setPixel(int x, int y, glm::vec3 color);
    glm::ivec2 transform(glm::ivec2& fromPoint, glm::ivec2& toPoint, int& mirrorX, int& mirrorY, bool& switched);

};