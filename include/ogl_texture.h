#pragma once

#include "ogl_presets.h"
#include "ogl_utils.h"
#include "ogl_shader.h"

inline int texCount = 0;

struct aTexture {
    unsigned int id;
    std::string type;
};

class cTexture
{
public:
    unsigned int id;
    int index;

    cTexture();
    ~cTexture();
    void set(int index, std::string address);
    cTexture(int index, std::string address);
    void run(Shader& ourShader, std::string texPath);
};