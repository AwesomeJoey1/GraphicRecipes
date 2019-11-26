#include "scenequadbresenham.h"

#include <iostream>

SceneQuadBresenham::SceneQuadBresenham()
{
    _shaderProg = new ShaderProgram("01_Basic");
    _imgSize = 32;
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
    naiveLineAlgorithm(glm::ivec2(-12, 0), glm::ivec2(12, 0), glm::vec3(1.0f,1.0f,1.0f));


    naiveLineAlgorithm(glm::ivec2(0,0), glm::ivec2(15,15), glm::vec3(1.0f,1.0f,0.0f));
    naiveLineAlgorithm(glm::ivec2(0,0), glm::ivec2(-15,-15), glm::vec3(1.0f,1.0f,0.0f));
    naiveLineAlgorithm(glm::ivec2(0,0), glm::ivec2(-15,15), glm::vec3(1.0f,1.0f,0.0f));
    naiveLineAlgorithm(glm::ivec2(0,0), glm::ivec2(15,-15), glm::vec3(1.0f,1.0f,0.0f));

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
    glm::vec2 translate = fromPoint;
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
        glm::vec2 mirroredPoint = glm::vec2(yMirror * x, round(xMirror * y));
        glm::vec2 finalPoint = mirroredPoint + translate;
        setPixel(finalPoint.x, finalPoint.y, color);
    }
}

void SceneQuadBresenham::bresenhamLine(glm::ivec2 fromPoint, glm::ivec2 toPoint, glm::vec3 color)
{


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
        // Else transform the y coordinate: translate(-imgSize) --> mirrorX(-1) --> -1 (for index reasons)
        y = (-1 * y + _coordSystemRange);
        int coord =y * _imgSize + x + _coordSystemRange;
        _image[y * _imgSize + x + _coordSystemRange] = color;
    }
}
