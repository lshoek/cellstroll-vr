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
#include "Jzon.h"
#define MAX_CELL_ELEMENTS 13

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
		struct FrameBufferObject
		{
			GLuint fboID;
			GLuint rboID;
			GLuint texID;
			GLuint byteDataTexID;
		};
		std::string* elementNames = new std::string[MAX_CELL_ELEMENTS];
		cTexture *cellTexture, *handTexture, *sliceTexture, *fingerTexture,
			*fistTexture, *normalmap_a, *punaiseTexture;
		cModel	*hand_model, *cell_model, *air_model, *pointer_model, *punaise_model;
		cModel *cellElements[MAX_CELL_ELEMENTS];
		LeapData leapData;
		LeapListener leapListener;
		Leap::Controller controller;
		PositionalDevice positionalDeviceCamera;
		GLuint lineShader;
		ShaderProgram *handShader, *pointerShader, *cellShader, *airShader, *fboShader, *punaiseShader;
		Light pointLight;
		Plane clippingPlane;
		FrameBufferObject fbo;
		clock_t clock_start;
		glm::vec2 screenSize;
		glm::vec3 center, cPlane;
		Label partLabel = Label("Nothing selected!");
		cFont* font = new cFont("Tahoma");
		float cellScale;
		int selectionIndex;
		enum ViewConfig { OCULUS_VIEW, SIMULATION_VIEW };
		ViewConfig viewConfig;
		bool selected = false;

		void loadTextures();
		void loadModels();
		void loadShaders();

		void displayText(int x, int y, std::string string);
		void drawBitmapText(std::string caption, int score, float r, float g, float b, float x, float y, float z);
		void setPositionalDevice(ViewConfig);

	public:
		CellStrollApp(void);
		~CellStrollApp(void);

		virtual void init();
		virtual void preFrame(double, double);
		virtual void draw(const glm::mat4 &, const glm::mat4 &);
		void drawText(const char *text, int length, int x, int y, int z);
		void fillInformationArray();
		void displayInformationCellPart(int part);
};