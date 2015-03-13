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

	checkers_model = CaveLib::loadModel("data/CellStroll/models/checkers_sphere.obj", new ModelLoadOptions(1.0f));
}

void App::preFrame(double, double totalTime)
{
	glm::mat4 mat = wand.getData();
}

void App::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	glEnable(GL_TEXTURE_2D);
	glTranslatef(-1.0f, 0.0f, -1.0f);
	glScalef(5.0f, 5.0f, 5.0f);

	checkers_model->draw();
	DrawWireFrame();
}

void App::DrawWireFrame(void)
{
	glPushMatrix();
	glLineWidth(1.0);
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(0.0f, -0.15f, 0.0f);
	for (float i = -2.0f; i <= 2.0f; i += 0.1f)
	{
		glBegin(GL_LINES);
		glVertex3f(i, 0.0f, -2.0f);
		glVertex3f(i, 0.0f, 2.0f);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(-2.0f, 0.0f, i);
		glVertex3f(2.0f, 0.0f, i);
		glEnd();
	}
	glPopMatrix();
}