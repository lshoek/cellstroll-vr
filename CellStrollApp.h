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
		struct FrameBufferObject
		{
			GLuint fboID;
			GLuint rboID;
			GLuint texID;
			GLuint byteDataTexID;
		};
		LeapData leapData;
		LeapListener leapListener;
		Leap::Controller controller;
		PositionalDevice positionalDeviceCamera;
		ShaderProgram *handShader, *pointerShader, *cellShader, *airShader, *fboShader;
		GLuint lineShader;
		cTexture *cellTexture, *handTexture, *sliceTexture, *fingerTexture, *fistTexture, *normalmap_a;
		cModel *hand_model, *cell_model, *air_model, *pointer_model,*punaise_model;
		// AnimalCell models
		cModel *centriole_model, *nucleolos_model, *flagellum_model, *golgi_model, *cytoplasm_model, *lysosome_model, *nucleus_model, *mitochondrion_model,
			*nuclearMembrane_model, *cellMembrane_model, *peroxisome_model, *reticulum_model, *filament_model;

		Light pointLight;
		Plane clippingPlane;
		FrameBufferObject fbo;
		clock_t clock_start;
		glm::vec2 screenSize;
		glm::vec3 center, cPlane;
		Label partLabel = Label("Nothing selected!");
		cFont* font = new cFont("Tahoma");
		float cellScale;

	public:
		enum ViewConfig { OCULUS_VIEW, SIMULATION_VIEW };
		CellStrollApp(void);
		~CellStrollApp(void);
		virtual void init();
		virtual void preFrame(double, double);
		virtual void draw(const glm::mat4 &, const glm::mat4 &);
		glm::vec3 extractCameraPosition(const glm::mat4 &);
		void displayText(int x, int y, std::string string);
		void drawBitmapText(std::string caption, int score, float r, float g, float b,float x, float y, float z);
		void setPositionalDevice(ViewConfig);
};