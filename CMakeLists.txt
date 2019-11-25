cmake_minimum_required(VERSION 3.13)
project(GraphicRecipes)

include_directories(Source, glad/include/glad, glm/glm, glfw/include)
include_directories(Source/Ingredients)

set(CMAKE_CXX_STANDARD 14)

link_libraries(GL GLU glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor)

add_executable(01_Basic Source/01_Basic/main.cpp Source/Ingredients/glad.c Source/01_Basic/scenetriangle.cpp Source/01_Basic/scenetriangle.h Source/Ingredients/scene.h Source/Ingredients/shaderprogram.cpp Source/Ingredients/shaderprogram.h Source/01_Basic/scenetriangletransforms.cpp Source/01_Basic/scenetriangletransforms.h Source/01_Basic/scenequad.cpp Source/01_Basic/scenequad.h Source/01_Basic/scenequadbresenham.cpp Source/01_Basic/scenequadbresenham.h)
add_executable(02_BasicLighting Source/02_BasicLighting/main.cpp Source/Ingredients/glad.c Source/Ingredients/shaderprogram.cpp Source/Ingredients/shaderprogram.h)


