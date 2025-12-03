#pragma once

#include <GL/glew.h>
#include "CGShaderProgram.h"
#include "CGScene.h"
#include "CGSkybox.h"
#include "CGCamera.h"
#include "CGObject.h"

class CGModel
{
public:
	void initialize(int w, int h);
	void finalize();
	void render();
	void update();
	void key_pressed(int key);
	void mouse_button(int button, int action);
	void mouse_move(double xpos, double ypos);
	void colocarCamara();
	void resize(int w, int h);

private:
	CGShaderProgram* sceneProgram;
	CGShaderProgram* skyboxProgram;
	CGShaderProgram* shadowProgram;
	CGScene* scene;
	CGObject* coche;
	CGCamera* camera;
	CGSkybox* skybox;
	glm::mat4 projection;
	bool fueraCoche = false;
	bool mAtras = false;
	bool fueraBalon = false;
	bool mAtrasB = false;

	GLsizei wndWidth;
	GLsizei wndHeight;
	GLuint shadowFBO;
	GLuint depthTexId;
	GLuint offsetTexId;

	bool InitShadowMap();
	void CarConstraints();
	void BallConstraints();
	void BallCarConstraints();
	void BuildOffsetTex(int texSize, int samplesU, int samplesV);
	float jitter();
};
