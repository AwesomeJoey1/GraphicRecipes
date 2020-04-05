#include "scenequadaliasing.h"

#include "scenequadbresenham.h"

#include <iostream>
#include <numeric>

SceneQuadAliasing::SceneQuadAliasing()
{
    _shaderProg = new ShaderProgram("01_Basic");
    _samplingScale = 2;
    _imgScale = 16;
    _imgSize = 32*_imgScale;
    _coordSystemRange = _imgSize / 2;
    _superSampledImage = std::vector<glm::vec3>(_imgSize *_samplingScale * _imgSize
            * _samplingScale, glm::vec3(0.0f));
    _image = std::vector<glm::vec3>(_imgSize*_imgSize, glm::vec3(0.0f));
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


    // draw coordinate system for better visualization
    //bresenhamLine(glm::ivec2(-12,0), glm::ivec2(12,0), glm::vec3(1.0f,1.0f,1.0f));
    //bresenhamLine(glm::ivec2(0,12), glm::ivec2(0,-12), glm::vec3(1.0f,1.0f,1.0f));

    //bresenhamLine(glm::ivec2(-8*_imgScale,8*_imgScale), glm::ivec2(8*_imgScale,-8*_imgScale), glm::vec3(1.0f,0.0f,0.0f), _samplingScale);
    //bresenhamLine(glm::ivec2(-32,32), glm::ivec2(-32,21), glm::vec3(1.0f,0.0f,0.0f), _samplingScale);
    //bresenhamLine(glm::ivec2(-32,32), glm::ivec2(-21,32), glm::vec3(1.0f,0.0f,0.0f), _samplingScale);


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


    //setPixel(0, 0, glm::vec3(1.0f, 0.0f, 0.0f), 2); // origin

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
                    r+= superSampledColor.r;
                    g+= superSampledColor.g;
                    b+= superSampledColor.b;
                }
            }
            // Averaging color and write to original image texture
            int numPixels = _samplingScale*_samplingScale;
            _image[j*_imgSize + i] = glm::vec3(r/numPixels, g/numPixels, b/numPixels);
        }
    }
}


/*
 * ################################### STANDARD BRESENHAM ###################################
 */
// Line drawing algorithm of Jack Bresenham.
// Decision of drawn pixel by evaluation of a decision variable based on the implicit line function
// Note: only integer operations are used
void SceneQuadAliasing::bresenhamLine(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color, unsigned int samplingScale)
{
    int mirrorX = 1;        // mirrors the point on the x axis
    int mirrorY = 1;        // mirrors the point on the y axis
    bool switched = false;  // switch of x and y

    fromPoint *= samplingScale;
    toPoint *= samplingScale;
    glm::ivec2 translate = transform(fromPoint, toPoint, mirrorX, mirrorY, switched);

    int x = 0;
    int y = 0;
    int deltaX = toPoint.x; // because fromPoint is translated to origin
    int deltaY = toPoint.y;
    int northEast = 2 * (deltaY - deltaX);
    int east = 2* deltaY;
    int d = 2* deltaY - deltaX;

    setPixel(translate.x, translate.y, color, samplingScale); // translate is the old starting point
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
            setPixel(mirrorY * y + translate.x, mirrorX * x + translate.y, color, samplingScale);
        } else {
            setPixel(mirrorY * x + translate.x, mirrorX * y + translate.y, color, samplingScale);
        }

    }
}

// Circle drawing algorithm of Jack Bresenham
// Again, decision variable for pixel drawing based on the implicit function of a circle.
void SceneQuadAliasing::bresenhamCircle(glm::ivec2 center, unsigned int radius, glm::vec3 color, unsigned int samplingScale) {
    //glm::ivec2 translate = center;

    radius *= samplingScale;

    int x = 0;
    int y = radius;
    int d = 5 - 4*radius;
    int deltaE, deltaSE;

    drawCirclePixels(x , y, center.x*samplingScale, center.y*samplingScale, color, samplingScale);

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
        drawCirclePixels(x , y, center.x*samplingScale, center.y*samplingScale, color, samplingScale);
    }
}

// Helper method for circle drawing
// Symmetry of a circle gives 7 further points to draw
void SceneQuadAliasing::drawCirclePixels(int x, int y, int cx, int cy, const glm::vec3& color, unsigned int samplingScale)
{
    setPixel(x + cx, y + cy, color, samplingScale);      // 1st -|
    setPixel(y + cx, x + cy, color, samplingScale);      // 2nd  |
    setPixel(-y + cx, x + cy, color, samplingScale);     // 3rd  |
    setPixel(-x + cx, y + cy, color, samplingScale);     // 4th  |===     OCTANT
    setPixel(-x + cx, -y + cy, color, samplingScale);    // 5th  |
    setPixel(-y + cx, -x + cy, color, samplingScale);    // 6th  |
    setPixel(y + cx, -x + cy, color, samplingScale);     // 7th  |
    setPixel(x + cx, -y + cy, color, samplingScale);     // 8th -|
}

// Helper method to set pixels of texture image
void SceneQuadAliasing::setPixel(int x, int y, glm::vec3 color, int samplingScale)
{
    // Sooo... OpenGL coordinate system origin for the texture is the upper left corner (Texture is flipped).
    // Positive y is down and positive x is right.
    // Therefore a little transformation is needed to display the texture/algorithm in a way we are used to.
    // In order to show the algorithms work on every octant, (0,0) is chosen to be the midpoint of the grid.
    // (!) origin of grid is 1 pixel off because of square texture.

    // First check whether the coordinates are inside the image boundary
    int range = (int) _coordSystemRange * samplingScale;
    if (x < -range || y < -range || x >= range || y > range)
    {
        std::cerr << "Index out of bounds for x = " << x << " and y = " << y << ". Coord range from: +-" << range << "\n";
    } else {
        // Else transform the y coordinate: mirror(y) --> translate(_coordSystemRange)
        y = (-1 * y + _coordSystemRange*samplingScale);
        //if (samplingScale > 1) {
        _superSampledImage[y * _imgSize * samplingScale + x + _coordSystemRange * samplingScale] = color;
        //} else {
        //    _image[y * _imgSize + x + _coordSystemRange] = color;
        //}
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

