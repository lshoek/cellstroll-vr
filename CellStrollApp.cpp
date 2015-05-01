#pragma comment(lib,"opengl32")
#include "CellStrollApp.h"
#define _USE_MATH_DEFINES

CellStrollApp::CellStrollApp(void){}
CellStrollApp::~CellStrollApp(void){}

void CellStrollApp::init(void)
{
	//CONFIGS
	simHead.init("MainUserHead");
	simCamera.init("CameraPosition");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//LIGHTING
	pointLight.position = glm::vec3(20.0f, 10.0f, 0.0f);
	pointLight.intensities = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight.ambientCoefficient = 0.75f;
	pointLight.attentuation = 0.2f;

	// TEXTURES
	normalmap_a = CaveLib::loadTexture("data/CellStroll/textures/normalmap3.png", new TextureLoadOptions(GL_FALSE));

	//MODELS
	pointer_model = CaveLib::loadModel("data/CellStroll/models/sphere.obj", new ModelLoadOptions(1.0f));
	cell_model = CaveLib::loadModel("data/CellStroll/models/sphere.obj", new ModelLoadOptions(10.0f));
	cube_model = CaveLib::loadModel("data/CellStroll/models/cube.obj", new ModelLoadOptions(100.0f));

	//SHADERS
	normalShader = new ShaderProgram("data/CellStroll/shaders/normal.vert", "data/CellStroll/shaders/normal.frag");
	normalShader->link();

	noiseShader = new ShaderProgram("data/CellStroll/shaders/noise.vert", "data/CellStroll/shaders/noise.frag");
	noiseShader->link();

	airShader = new ShaderProgram("data/CellStroll/shaders/airnoise.vert", "data/CellStroll/shaders/airnoise.frag");
	airShader->link();

	//LEAP
	controller.addListener(leapListener);
	leapListener.setLeapData(&leapData);
	leapListener.onInit(controller);
}

void CellStrollApp::preFrame(double, double totalTime)
{
	GLfloat timeFctr = GLfloat(clock() - clock_start) / CLOCKS_PER_SEC; // calculate time(s) elapsed since last frame
	clock_start = clock();
	leapListener.setTimeDiff(timeFctr);
}

void CellStrollApp::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	// CONFIGS
	glEnable(GL_COLOR_MATERIAL);

	// UPDATE TIME UNIFORM
	GLfloat time = GLfloat(clock()) / GLfloat(CLOCKS_PER_SEC);

	// MVP
	glm::mat4 mvp = projectionMatrix * modelViewMatrix;
	glm::mat4 pointerMvp = glm::translate(mvp, rescaledFingerPosition(leapData.fingerPosition));
	glm::mat4 cellMvp = glm::translate(mvp, glm::vec3(0.0f, -5.0f, -15.0f));
	glm::mat4 airMvp = glm::translate(mvp, glm::vec3(-50.0f, -50.0f, 50.0f));

	// DRAW POINTER
	normalShader->use();
	glDisable(GL_TEXTURE_2D);
	normalShader->setUniformMatrix4("modelViewMatrix", modelViewMatrix);
	normalShader->setUniformVec3("light.position", pointLight.position);
	normalShader->setUniformVec3("light.intensities", pointLight.intensities);
	normalShader->setUniformFloat("light.attenuation", pointLight.attentuation);
	normalShader->setUniformFloat("light.ambientCoefficient", pointLight.ambientCoefficient);
	normalShader->setUniformFloat("time", time);
	normalShader->setUniformVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
	normalShader->setUniformVec3("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	normalShader->setUniformFloat("materialShininess", 5.0f);
	normalShader->setUniformVec3("cameraPosition", extractCameraPosition(simCamera.getData()));
	normalShader->setUniformMatrix4("modelViewProjectionMatrix", pointerMvp);
	pointer_model->draw(normalShader);

	// DRAW CELL
	noiseShader->use();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, normalmap_a->tid());
	noiseShader->setUniformInt("s_texture", 0);
	noiseShader->setUniformMatrix4("modelViewMatrix", modelViewMatrix);
	noiseShader->setUniformVec3("light.position", pointLight.position);
	noiseShader->setUniformVec3("light.intensities", pointLight.intensities);
	noiseShader->setUniformFloat("light.attenuation", pointLight.attentuation);
	noiseShader->setUniformFloat("light.ambientCoefficient", pointLight.ambientCoefficient);
	noiseShader->setUniformFloat("time", time);
	noiseShader->setUniformVec3("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	noiseShader->setUniformFloat("materialShininess", 5.0f);
	noiseShader->setUniformVec3("cameraPosition", extractCameraPosition(simCamera.getData()));
	noiseShader->setUniformMatrix4("modelViewProjectionMatrix", cellMvp);
	cell_model->draw(noiseShader);

	// DRAW AIR
	airShader->use();
	airShader->setUniformFloat("time", time);
	airShader->setUniformMatrix4("modelViewProjectionMatrix", airMvp);
	cube_model->draw(airShader);

	glUseProgram(0);
}

glm::vec3 CellStrollApp::extractCameraPosition(const glm::mat4 &modelView)
{
	glm::mat3 rotMat = glm::mat3(modelView);
	glm::vec3 d(modelView[3]);
	return -d * rotMat;
}

glm::vec3 CellStrollApp::rescaledFingerPosition(glm::vec3 fingerPos)
{
	return glm::vec3((fingerPos.x/4), (fingerPos.y/4)-50.0f, (fingerPos.z/4));
}