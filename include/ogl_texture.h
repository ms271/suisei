#pragma once

#include "ogl_presets.h"
#include "ogl_utils.h"
#include "ogl_shader.h"

class texture
{
public:
    unsigned int id;
    int index;

    texture();
    void set(int index, std::string address);
    texture(int index, std::string address);
    void run(shader& ourShader);
};