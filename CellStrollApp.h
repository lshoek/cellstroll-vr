#pragma once
#include <Windows.h>
#include <GL\glew.h>
#include <VrLib\Application.h>
#include <VrLib\Device.h>
#include <VrLib\Kernel.h>
#include <CaveLib\CaveLib.h>
#include <CaveLib\texture.h>
#include <CaveLib\Shader.h>
#include <CaveLib\model.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm\ext.hpp>
#include <iostream>
#include <ctime>
#include <thread>
#include "LeapListener.h"
#include "LeapData.h"
#include <CaveLib\ModelViewer\glut\glut.h>
#include <CaveLib\Components\Label.h>
#include <CaveLib\font.h>
#include "ToolBox.h"

class cTexture;

class CellStrollApp : public Application
{
	private:
		struct Light
		{
			glm::vec3 position;
			glm::vec3 intensities;
			float ambientCoefficient;
			float attentuation;
		};
		struct Plane
		{
			glm::vec3 point;
			glm::vec3 normal;
		};
		LeapData leapData;
		LeapListener leapListener;
		Leap::Controller controller;
		PositionalDevice simHead, simCamera;
		ShaderProgram *simpleShader, *cellShader, *airShader;
		GLuint pointShader;
		cTexture *cellTexture, *handTexture, *sliceTexture, *fingerTexture, *fistTexture, *normalmap_a;
		cModel *hand_model, *cell_model, *cube_model;
		Light pointLight;
		Plane clippingPlane;
		clock_t clock_start;
		glm::vec3 cPlane;
		Label partLabel = Label("Nothing selected!");
		float xScale, yScale, zScale;

	public:
		CellStrollApp(void);
		~CellStrollApp(void);
		virtual void init();
		virtual void preFrame(double, double);
		virtual void draw(const glm::mat4 &, const glm::mat4 &);
		glm::vec3 extractCameraPosition(const glm::mat4&);
		glm::vec3 rescaledPalmPosition(glm::vec3 fingerPos);
		void displayText(int x, int y, std::string string);
};