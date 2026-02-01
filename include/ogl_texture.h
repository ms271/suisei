#pragma once

#include "ogl_presets.h"
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