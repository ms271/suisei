#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class camera
{
	float fov = 45.0f;

	float yaw = -90.0f;
	float pitch = 0.0f;

	float lastX;// = SCR_WIDTH / 2;
	float lastY;// = SCR_HEIGHT / 2;
	const float sensitivity = 0.01f;
	float cam_speed = 2.0f;

	bool firstMouse = 1;

	camera(int width, int height)
	{
		lastX = ((float)width) / 2;
		lastY = ((float)height) / 2;
	}
};