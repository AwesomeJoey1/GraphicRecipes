#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scenetriangle.h"
#include "scenetriangletransforms.h"
#include "scenequad.h"
#include "scenequadbresenham.h"
#include "scenequadaliasing.h"

#include <string>
#include <iostream>

int WINDOW_WIDTH = 1080;
int WINDOW_HEIGHT = 1080;

GLFWwindow* _window;
Scene* _scene;

GLuint _vao;

std::string parseArgs(int argc, char** argv);
void printInfo(const char* exeFile);


static void errorCallback(int error, const char* description)
{
    std::cerr << "GLFW ERROR: " << description << "\n";
    exit(EXIT_FAILURE);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (_scene)
    {
        if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
            _scene->animate(!_scene->animating());
    }
}

void initialize()
{
    _scene->init();
}

void mainLoop()
{
    while( !glfwWindowShouldClose(_window) && glfwGetKey(_window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        _scene->update(glfwGetTime());
        _scene->render();

        glfwSwapBuffers(_window);
        glfwPollEvents();
    }

}

void resize(int width, int height)
{
    _scene->resize(width, height);
}

int main(int argc, char** argv) {

    std::string recipe = parseArgs(argc, argv);


    glfwSetErrorCallback(errorCallback);
    if (!glfwInit())
    {
        std::cerr << "ERROR: Failed to init GLFW.\n";
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    _window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, recipe.c_str(), NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        std::cerr << "ERROR: Failed to create GLFW window.\n";
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(_window);
    glfwSetKeyCallback(_window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        glfwTerminate();
        std::cerr << "ERROR: Failed to load glad extension.\n";
        exit(EXIT_FAILURE);
    }

    initialize();
    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    mainLoop();


    glfwTerminate();
    return 0;
}

std::string parseArgs(int argc, char** argv) {
    if (argc < 2) {
        printInfo(argv[0]);
        exit(0);
    }

    std::string recipe = argv[1];

    if(recipe == "triangle")
    {
        _scene = new SceneTriangle();
    } else if (recipe =="triangle-transforms")
    {
        _scene = new SceneTriangleTransforms();
    } else if(recipe == "quad")
    {
        _scene = new SceneQuad();
    } else if(recipe == "quad-bresenham")
    {
        _scene = new SceneQuadBresenham();
    } else if(recipe == "quad-aliasing")
    {
        _scene = new SceneQuadAliasing();
    }
    else {
        std::cerr << "Unknown recipe: " << recipe << "\n";
        exit(EXIT_FAILURE);
    }

    return recipe;
}

void printInfo(const char* exeFile)
{

    std::cout << "\n*****************************************************************************************\n"
                << "Usage: " << exeFile << ": recipe-name\n"
                << "*****************************************************************************************\n\n"
                << "Available recipes:\n"
                << "triangle:\t\t\tSimple triangle application.\n"
                << "triangle-transforms:\t\tTriangle application with affine transformations.\n"
                << "quad:\t\t\t\tSimple full screen quad application.\n"
                << "quad-bresenham:\t\t\tDrawing with Bresenham algorithm on full screen quad.\n"
                << "quad-aliasing:\t\t\tDisplay of different anti-aliasing methods.\n"
                << "further...\n";
}