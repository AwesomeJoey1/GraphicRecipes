#include "scenetriangletransforms.h"

SceneTriangleTransforms::SceneTriangleTransforms(){}

void SceneTriangleTransforms::init() {}

void SceneTriangleTransforms::update(float t) {}

void SceneTriangleTransforms::render() {}

void SceneTriangleTransforms::resize(int width, int height)
{
    glViewport(0,0, width, height);
}
