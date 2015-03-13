#pragma comment(lib,"opengl32")
#include <GL\glew.h>
#include <Windows.h>
#include <iostream>
#include "App.h"
#include <CaveLib\CaveLib.h>
#include <CaveLib\texture.h>
#include <CaveLib\model.h>

App::App(void){}
App::~App(void){}

void App::init(void)
{
	wand.init("WandPosition");
	head.init("MainUserHead");
	leftButton.init("LeftButton");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	brickwall_texture = CaveLib::loadTexture("data/CellStroll/textures/brickwall.jpg");
	checkers_model = CaveLib::loadModel("data/CellStroll/models/checkers_sphere.obj", new ModelLoadOptions(1.0f));
}

void App::preFrame(double, double totalTime)
{
	glm::mat4 mat = wand.getData();
}

void App::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	/* TEXTURES OFF */
	//glDisable(GL_TEXTURE_2D);
	//glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_TEXTURE_2D);
	glTranslatef(-1.0f, 0.0f, -1.0f);
	glScalef(5.0f, 5.0f, 5.0f);

	checkers_model->draw();
	DrawBrickWall();
}

void App::DrawBrickWall(void)
{
	glBindTexture(GL_TEXTURE_2D, brickwall_texture->tid());
	glPushMatrix();
	glBegin(GL_QUADS);

	// Top 
	glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);

	// Bottom 
	glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	// Front 
	glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face
	glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face 
	glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);

	// Right face
	glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();
	glPopMatrix();
}