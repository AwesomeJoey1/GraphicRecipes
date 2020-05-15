#include "scenequadaliasing.h"

#include "scenequadbresenham.h"

#include <iostream>
#include <numeric>

SceneQuadAliasing::SceneQuadAliasing()
{
    _shaderProg = new ShaderProgram("01_Basic");
    _samplingScale = 5;
    _imgSize = 30;
    _superSampledImage = std::vector<glm::vec3>(_imgSize *_samplingScale * _imgSize
            * _samplingScale, glm::vec3(1.0f));
    _image = std::vector<glm::vec3>(_imgSize*_imgSize, glm::vec3(0.0f));

    _filter3x3 = {1, 2, 1,
                  2, 4, 2,
                  1, 2, 1};
    _filter5x5 = {1, 2, 3, 2, 1,
                  2, 4, 6, 4, 2,
                  3, 6, 9, 6, 3,
                  2, 4, 6, 4, 2,
                  1, 2, 3, 2, 1};

    _filter7x7 = {1, 2, 3, 4, 3, 2, 1,
                  2, 4, 6, 8, 6, 4, 2,
                  3, 6, 9, 12, 9, 6, 3,
                  4, 8, 12, 16, 12, 8, 4,
                  3, 6, 9, 12, 9, 6, 3,
                  2, 4, 6, 8, 6, 4, 2,
                  1, 2, 3, 4, 3, 2, 1};
}

void SceneQuadAliasing::init()
{
    loadAndCompileShaders();

    const float quad[] = {
            //  Position           Tex-Coords
            -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 1.0f };

    const short indices[] = { 0, 1, 2,
                              0, 3, 1};

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    GLuint vbo, ebo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Tex-Coords (offset of 3*float because of position values)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //bresenhamLine(glm::ivec2(-8*_imgScale, 8*_imgScale), glm::ivec2(8*_imgScale,8*_imgScale), glm::vec3(1.0f,0.0f,1.0f), _samplingScale);
    //bresenhamLine(glm::ivec2(-8*_imgScale, -8*_imgScale), glm::ivec2(-8*_imgScale,8*_imgScale), glm::vec3(1.0f,0.0f,1.0f), _samplingScale);
    bresenhamLine(glm::ivec2(0,(_imgSize)*_samplingScale-1),
            glm::ivec2((_imgSize/4)*_samplingScale-1, 0), glm::vec3(0.0f,0.0f,0.0f));

    /*
    //head
    bresenhamCircle(glm::ivec2(0,0), 12*_imgScale, glm::vec3(1.0f, 0.4f, 0.0f), _samplingScale);

    // eyes
    bresenhamCircle(glm::ivec2(4*_imgScale,5*_imgScale), 3*_imgScale, glm::vec3(0.0f, 0.4f, 1.0f), _samplingScale);
    bresenhamCircle(glm::ivec2(4*_imgScale,5*_imgScale), 1*_imgScale, glm::vec3(0.0f, 0.0f, 1.0f), _samplingScale);
    bresenhamCircle(glm::ivec2(-4*_imgScale,5*_imgScale), 3*_imgScale, glm::vec3(0.0f, 0.4f, 1.0f), _samplingScale);
    bresenhamCircle(glm::ivec2(-4*_imgScale,5*_imgScale), 1*_imgScale, glm::vec3(0.0f, 0.0f, 1.0f), _samplingScale);

    // nose
    bresenhamLine(glm::ivec2(0*_imgScale,2*_imgScale), glm::ivec2(-3*_imgScale,-1*_imgScale), glm::vec3(1.0f,0.0f,1.0f), _samplingScale);
    bresenhamLine(glm::ivec2(-3*_imgScale,-1*_imgScale), glm::ivec2(0*_imgScale,-1*_imgScale), glm::vec3(1.0f,0.0f,1.0f), _samplingScale);

    // mouth
    bresenhamLine(glm::ivec2(-4*_imgScale, -5*_imgScale), glm::ivec2(-2*_imgScale,-6*_imgScale), glm::vec3(1.0f,0.0f,1.0f), _samplingScale);
    bresenhamLine(glm::ivec2(-2*_imgScale, -6*_imgScale), glm::ivec2(2*_imgScale,-6*_imgScale), glm::vec3(1.0f,0.0f,1.0f), _samplingScale);
    bresenhamLine(glm::ivec2(2*_imgScale, -6*_imgScale), glm::ivec2(4*_imgScale,-5*_imgScale), glm::vec3(1.0f,0.0f,1.0f), _samplingScale);
    */
    // Downsample  to the original resolution
    downsample();

    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _imgSize, _imgSize, 0, GL_RGB, GL_FLOAT, &_image[0]);

    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _imgSize * _samplingScale, _imgSize * _samplingScale, 0, GL_RGB, GL_FLOAT, &_superSampledImage[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _imgSize, _imgSize, 0, GL_RGB, GL_FLOAT, &_image[0]);

    _shaderProg->setUniform("TexQuad", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindVertexArray(0);
}

void SceneQuadAliasing::update(float t) {

}

void SceneQuadAliasing::render() {
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void SceneQuadAliasing::resize(int width, int height)
{
    glViewport(0,0,width, height);
}

/*
 * ********************************* PRIVATE *********************************
 */

void SceneQuadAliasing::loadAndCompileShaders()
{
    _shaderProg->loadShader("shader/quadaliasing.vert");
    _shaderProg->loadShader("shader/quadaliasing.frag");
    _shaderProg->link();
    _shaderProg->use();
}

/*
 * ###################################    Downsampling    ###################################
 */
void SceneQuadAliasing::downsample()
{
    int filterSize = 5;
    // Iterating over every pixel of the original image
    for (int i = 0; i < _imgSize; i++)
    {
        for (int j = 0; j < _imgSize; j++)
        {
            float r = 0, g = 0, b = 0;
            // Iterating over the supersampled part
            for (int k = 0; k < _samplingScale; k++)
            {
                for (int l = 0; l < _samplingScale; l++)
                {
                    int idx = (j*_samplingScale + l)*_imgSize*_samplingScale + _samplingScale*i+k;
                    glm::vec3 superSampledColor = _superSampledImage[idx];
                    r+= _filter5x5[l*_samplingScale + k] * superSampledColor.r;
                    b+= _filter5x5[l*_samplingScale + k] * superSampledColor.b;
                    g+= _filter5x5[l*_samplingScale + k] * superSampledColor.g;
                }
            }
            // Averaging color and write to original image texture
            auto sum = std::accumulate(_filter5x5.begin(), _filter5x5.end(), decltype(_filter5x5)::value_type(0));
            float numPixels = static_cast<float>(sum);
            _image[j*_imgSize + i] = glm::vec3(r/static_cast<float>(numPixels),
                                               g/static_cast<float>(numPixels),
                                               b/static_cast<float>(numPixels));
        }
    }
}


/*
 * ################################### STANDARD BRESENHAM ###################################
 */
// Line drawing algorithm of Jack Bresenham.
// Decision of drawn pixel by evaluation of a decision variable based on the implicit line function
// Note: only integer operations are used
void SceneQuadAliasing::bresenhamLine(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color)
{
    int mirrorX = 1;        // mirrors the point on the x axis
    int mirrorY = 1;        // mirrors the point on the y axis
    bool switched = false;  // switch of x and y

    glm::ivec2 translate = transform(fromPoint, toPoint, mirrorX, mirrorY, switched);

    int x = 0;
    int y = 0;
    int deltaX = toPoint.x; // because fromPoint is translated to origin
    int deltaY = toPoint.y;
    int northEast = 2 * (deltaY - deltaX);
    int east = 2* deltaY;
    int d = 2* deltaY - deltaX;

    setPixel(translate.x, translate.y, color); // translate is the old starting point
    while (x < toPoint.x)
    {
        if (d >= 0)
        {
            d+= northEast;
            x++;
            y++;
        } else {
            d+= east;
            x++;
        }

        if (switched)
        {
            setPixel(mirrorY * y + translate.x, mirrorX * x + translate.y, color);
        } else {
            setPixel(mirrorY * x + translate.x, mirrorX * y + translate.y, color);
        }

    }
}

// Circle drawing algorithm of Jack Bresenham
// Again, decision variable for pixel drawing based on the implicit function of a circle.
void SceneQuadAliasing::bresenhamCircle(glm::ivec2 center, unsigned int radius, glm::vec3 color) {
    //glm::ivec2 translate = center;

    int x = 0;
    int y = radius;
    int d = 5 - 4*radius;
    int deltaE, deltaSE;

    drawCirclePixels(x , y, center.x, center.y, color);

    while (y > x)
    {
        if (d >= 0)
        {
            deltaSE = 4 * (2*(x-y) + 5);
            d += deltaSE;
            x++;
            y--;
        } else {
            deltaE = 4 * (2*x +3);
            d += deltaE;
            x++;
        }
        drawCirclePixels(x , y, center.x, center.y, color);
    }
}

// Helper method for circle drawing
// Symmetry of a circle gives 7 further points to draw
void SceneQuadAliasing::drawCirclePixels(int x, int y, int cx, int cy, const glm::vec3& color)
{
    setPixel(x + cx, y + cy, color);      // 1st -|
    setPixel(y + cx, x + cy, color);      // 2nd  |
    setPixel(-y + cx, x + cy, color);     // 3rd  |
    setPixel(-x + cx, y + cy, color);     // 4th  |===     OCTANT
    setPixel(-x + cx, -y + cy, color);    // 5th  |
    setPixel(-y + cx, -x + cy, color);    // 6th  |
    setPixel(y + cx, -x + cy, color);     // 7th  |
    setPixel(x + cx, -y + cy, color);     // 8th -|
}

// Helper method to set pixels of texture image
void SceneQuadAliasing::setPixel(int x, int y, glm::vec3 color)
{
    // Sooo... OpenGL coordinate system origin for the texture is the upper left corner (Texture is flipped).
    // Positive y is down and positive x is right.
    // Therefore a little transformation is needed to display the texture/algorithm in a way we are used to.

    // First check whether the coordinates are inside the image boundary
    if (x < 0 || y < 0 || x >= _imgSize * _samplingScale || y >= _imgSize * _samplingScale)
    {
        std::cerr << "Index out of bounds for x = " << x << " and y = " << y << ". Coord range from: 0 to " << _imgSize*_samplingScale << "\n";
    } else {
        y = (-1 * y + _imgSize*_samplingScale-1);
        _superSampledImage[y * _imgSize*_samplingScale + x] = color;
    }
}

// Helper method for transforming random position to first octant
glm::ivec2 SceneQuadAliasing::transform(glm::ivec2& fromPoint, glm::ivec2& toPoint, int& mirrorX, int& mirrorY, bool& switched)
{
    glm::ivec2 translate = fromPoint;
    fromPoint -= translate; // --> (0,0)
    toPoint -= translate;

    int x = toPoint.x;
    int y = toPoint.y;

    if (toPoint.x < 0)
        mirrorY = -1;
    if (toPoint.y < 0)
        mirrorX = -1;
    if (abs(toPoint.x) < abs(toPoint.y))
        switched = true;

    toPoint.x *= mirrorY;
    toPoint.y *= mirrorX;

    if (switched)
    {
        int tmp = toPoint.x;
        toPoint.x = toPoint.y;
        toPoint.y = tmp;
    }

    return translate;
}

