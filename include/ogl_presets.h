#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

inline unsigned int SCR_WIDTH = 1920;
inline unsigned int SCR_HEIGHT = 1080;

inline const unsigned int MAJOR_VERSION = 3;
inline const unsigned int MINOR_VERSION = 3;

inline const float window_factor = 1;

inline bool BgColorStatus = 0;