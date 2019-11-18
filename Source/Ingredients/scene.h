#pragma once

class Scene
{
public:
    Scene() : _animating(true) {}

    virtual void init() = 0;

    virtual void update(float t) = 0;

    virtual void render() = 0;

    virtual void resize(int width, int height) = 0;

    void animate(bool a) { _animating = a; };

    bool animating() { return _animating; }

protected:
    bool _animating;
};