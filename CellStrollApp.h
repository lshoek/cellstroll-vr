#pragma once
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
		PositionalDevice simHead, simCamera;
		ShaderProgram *noiseShader, *airShader;
		cTexture *normalmap_a;
		cModel *cell_model, *cube_model;
		Light pointLight;
		clock_t clock_start;
		int fps;

	public:
		CellStrollApp(void);
		~CellStrollApp(void);
		virtual void init();
		virtual void preFrame(double, double);
		virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix);
		glm::vec3 extractCameraPosition(const glm::mat4&);
};