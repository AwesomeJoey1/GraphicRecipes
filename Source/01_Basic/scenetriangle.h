#pragma once

#include "scene.h"

class SceneTriangle : public Scene
{
public:

    SceneTriangle();

    void init();

    void update(float t);

    void render();

    void resize(int width, int height);

};