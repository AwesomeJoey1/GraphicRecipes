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
    unsigned int _imgScale;
    unsigned int _coordSystemRange;
    std::vector<glm::vec3> _image, _superSampledImage;


    void loadAndCompileShaders();

    void downsample();

    void bresenhamLine(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color, unsigned int samplingScale=1);
    void bresenhamCircle(glm::ivec2 center, unsigned int radius, glm::vec3 color, unsigned int samplingScale=1);
    void drawCirclePixels(int x, int y, int cx, int cy, const glm::vec3& color, unsigned int samplingScale);
    void setPixel(int x, int y, glm::vec3 color, int samplingScale=1);
    glm::ivec2 transform(glm::ivec2& fromPoint, glm::ivec2& toPoint, int& mirrorX, int& mirrorY, bool& switched);

};