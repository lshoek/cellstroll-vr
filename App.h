#pragma once

#include <VrLib\Application.h>
#include <VrLib\Device.h>
#include <VrLib\Kernel.h>
#include <CaveLib\CaveLib.h>
#include <GL\glew.h>

class cTexture;

class App : public Application
{
	private:
		PositionalDevice wand;
		PositionalDevice head;
		DigitalDevice leftButton;
		cTexture* brickwall_texture;
		cModel* checkers_model;

	public:
		App(void);
		~App(void);
		virtual void init();
		virtual void preFrame(double, double);
		virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix);
		void DrawBrickWall();
};