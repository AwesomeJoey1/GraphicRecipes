#include "scenequadbresenham.h"

#include <iostream>

SceneQuadBresenham::SceneQuadBresenham()
{
    _shaderProg = new ShaderProgram("01_Basic");
    _imgSize = 64;
    _coordSystemRange = _imgSize / 2;
    _image = std::vector<glm::vec3>(_imgSize*_imgSize, glm::vec3(0.0f));
}

void SceneQuadBresenham::init()
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

    //head
    bresenhamCircle(glm::ivec2(0,0), 28, glm::vec3(1.0f, 0.4f, 0.0f));


    // eyes
    bresenhamCircle(glm::ivec2(10,10), 5, glm::vec3(0.0f, 0.4f, 1.0f));
    bresenhamCircle(glm::ivec2(10,10), 1, glm::vec3(0.0f, 0.0f, 1.0f));
    bresenhamCircle(glm::ivec2(-10,10), 5, glm::vec3(0.0f, 0.4f, 1.0f));
    bresenhamCircle(glm::ivec2(-10,10), 1, glm::vec3(0.0f, 0.0f, 1.0f));

    // eyebrows
    bresenhamLine(glm::ivec2(18,15), glm::ivec2(10,17), glm::vec3(1.0f,1.0f,1.0f));
    bresenhamLine(glm::ivec2(10,17), glm::ivec2(3,14), glm::vec3(1.0f,1.0f,1.0f));

    bresenhamLine(glm::ivec2(-20,15), glm::ivec2(-15,20), glm::vec3(1.0f,1.0f,1.0f));
    bresenhamLine(glm::ivec2(-15,20), glm::ivec2(-3,22), glm::vec3(1.0f,1.0f,1.0f));

    // mouth
    bresenhamLine(glm::ivec2(-13, -15), glm::ivec2(0,-9), glm::vec3(1.0f,1.0f,1.0f));
    bresenhamLine(glm::ivec2(0, -9), glm::ivec2(8,-11), glm::vec3(1.0f,1.0f,1.0f));




    /*naiveLineAlgorithm(glm::ivec2(0,0), glm::ivec2(15,15), glm::vec3(1.0f,1.0f,0.0f));
    naiveLineAlgorithm(glm::ivec2(0,0), glm::ivec2(-15,-15), glm::vec3(1.0f,1.0f,0.0f));
    naiveLineAlgorithm(glm::ivec2(0,0), glm::ivec2(-15,15), glm::vec3(1.0f,1.0f,0.0f));
    naiveLineAlgorithm(glm::ivec2(0,0), glm::ivec2(15,-15), glm::vec3(1.0f,1.0f,0.0f));
*/
    // 8 octant test
    /*bresenhamLine(glm::ivec2(1,2), glm::ivec2(5,7), glm::vec3(1.0f,1.0f,0.4f));
    bresenhamLine(glm::ivec2(2,1), glm::ivec2(7,5), glm::vec3(1.0f,1.0f,0.4f));
    bresenhamLine(glm::ivec2(-1,2), glm::ivec2(-5,7), glm::vec3(1.0f,1.0f,0.4f));
    bresenhamLine(glm::ivec2(-2,1), glm::ivec2(-7,5), glm::vec3(1.0f,1.0f,0.4f));
    bresenhamLine(glm::ivec2(-1,-2), glm::ivec2(-5,-7), glm::vec3(1.0f,1.0f,0.4f));
    bresenhamLine(glm::ivec2(-2,-1), glm::ivec2(-7,-5), glm::vec3(1.0f,1.0f,0.4f));
    bresenhamLine(glm::ivec2(1,-2), glm::ivec2(5,-7), glm::vec3(1.0f,1.0f,0.4f));
    bresenhamLine(glm::ivec2(2,-1), glm::ivec2(7,-5), glm::vec3(1.0f,1.0f,0.4f));*/

    setPixel(0, 0, glm::vec3(1.0f, 0.0f, 0.0f)); // origin

    GLuint tex;
    glGenTextures(1, &tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    // ToDo: create empty Texture first. At render time populate
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _imgSize, _imgSize, 0, GL_RGB, GL_FLOAT, &_image[0]);

    _shaderProg->setUniform("TexQuad", 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glBindVertexArray(0);
}

void SceneQuadBresenham::update(float t) {

}

void SceneQuadBresenham::render() {
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}

void SceneQuadBresenham::resize(int width, int height)
{
    glViewport(0,0,width, height);
}

/*
 * ********************************* PRIVATE *********************************
 */

void SceneQuadBresenham::loadAndCompileShaders()
{
    _shaderProg->loadShader("shader/quadbresenham.vert");
    _shaderProg->loadShader("shader/quadbresenham.frag");
    _shaderProg->link();
    _shaderProg->use();
}

void SceneQuadBresenham::naiveLineAlgorithm(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color)
{
    glm::ivec2 translate = fromPoint;
    fromPoint = glm::ivec2(0);
    toPoint -= translate;

    int xMirror = 1;
    int yMirror = 1;
    if (toPoint.x < fromPoint.x)
    {
        yMirror = -1;
        toPoint.x *= yMirror;
    }

    if (toPoint.y < fromPoint.y)
    {
        xMirror = -1;
        toPoint.y *= xMirror;
    }


    if ((toPoint.x - fromPoint.x) == 0)
    {
        std::cout << "WARNING: Slope is infinite. Naive doesn't work on vertical lines!\n";
        return;
    }

    float m = (toPoint.y - fromPoint.y) / (float)(toPoint.x - fromPoint.x);

    for (int x = 0; x <= toPoint.x; x++)
    {
        double y = m * x;
        glm::ivec2 mirroredPoint = glm::ivec2(yMirror * x, round(xMirror * y));
        glm::ivec2 finalPoint = mirroredPoint + translate;
        setPixel(finalPoint.x, finalPoint.y, color);
    }
}

void SceneQuadBresenham::bresenhamLine(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color)
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
            int a = mirrorY * y + translate.x;
            int b = mirrorX * x + translate.y;
            setPixel(mirrorY * y + translate.x, mirrorX * x + translate.y, color);
        } else {
            int a = mirrorY * x + translate.x;
            int b = mirrorX * y + translate.y;
            setPixel(mirrorY * x + translate.x, mirrorX * y + translate.y, color);
        }

    }
}

void SceneQuadBresenham::bresenhamCircle(glm::ivec2 center, unsigned int radius, glm::vec3 color) {
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

void SceneQuadBresenham::drawCirclePixels(int x, int y, int cx, int cy, glm::vec3 color)
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


void SceneQuadBresenham::setPixel(int x, int y, glm::vec3 color)
{
    // Sooo... OpenGL coordinate system origin for the texture is the upper left corner (Texture is flipped).
    // Positive y is down and positive x is right.
    // Therefore a little transformation is needed to display the texture/algorithm in a way we are used to.
    // In order to show the algorithms work on every octant, (0,0) is chosen to be the midpoint of the grid.
    // (!) origin of grid is 1 pixel off because of square texture.

    // First check whether the coordinates are inside the image boundary
    int range = (int) _coordSystemRange;
    if (x < -range || y < -range || x >= range || y >= range)
    {
        std::cerr << "Index out of bounds for x = " << x << " and y = " << y << ". Coord range from: +-" << _coordSystemRange << "\n";
    } else {
        // Else transform the y coordinate: mirror(y) --> translate(_coordSystemRange)
        y = (-1 * y + _coordSystemRange);
        _image[y * _imgSize + x + _coordSystemRange] = color;
    }
}


glm::ivec2 SceneQuadBresenham::transform(glm::ivec2& fromPoint, glm::ivec2& toPoint, int& mirrorX, int& mirrorY, bool& switched)
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

