#ifndef OGL_TEXTURE
#define OGL_TEXTURE

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "ogl_utils.h"
#include "ogl_shader.h"

class ogl_texture
{
public:
    unsigned int texture;
    int index;

    ogl_texture(int index, std::string address);
    void run();
};

#endif