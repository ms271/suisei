#pragma once
#include"ogl_presets.h"
#include"ogl_utils.h"
#include"ogl_shader.h"
#include"ogl_cam.h"
#include"ogl_object.h"

class testScene
{
public:
	initWindow* sceneWindow;
	Shader* sceneShader;
	camera* sceneCamera;

	testScene(initWindow& thisWindow, Shader& thisShader, camera& thisCamera);
	void run();
	void resultExport();
};