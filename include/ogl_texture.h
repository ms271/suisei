#pragma once

#include "ogl_presets.h"
#include "ogl_utils.h"
#include "ogl_shader.h"

inline int texCount = 0;

class texture
{
public:
    unsigned int id;
    int index;
    std::string path;
    std::string type;

    texture();
    ~texture();
    texture(int index, std::string address);

    void run(shader& ourShader, std::string texPath) const;
};
