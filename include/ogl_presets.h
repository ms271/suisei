#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

inline unsigned int SCR_WIDTH = 1920;
inline unsigned int SCR_HEIGHT = 1080;

inline const unsigned int MAJOR_VERSION = 4;
inline const unsigned int MINOR_VERSION = 6;

inline float FOV = 45.0f;

inline glm::mat4 PROJ = glm::perspective(glm::radians(FOV), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);;

inline float YAW = -90.0f;
inline float PITCH = 0.0f;

inline float LAST_X = SCR_WIDTH/2;
inline float LAST_Y = SCR_HEIGHT/2;
inline const float SENSITIVITY = 0.01f;
inline float CAM_BASE_SPEED = 2.0f;

inline bool FIRST_MOUSE = 1;
