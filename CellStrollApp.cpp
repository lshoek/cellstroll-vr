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

	//CLIPPING PLANE
	clippingPlane.point = glm::vec3(100.0f, 0.0f, 0.0f);
	clippingPlane.normal = glm::vec3(0.0f, -1.0f, 0.0f);

	// TEXTURES
	normalmap_a = CaveLib::loadTexture("data/CellStroll/textures/normalmap3.png", new TextureLoadOptions(GL_FALSE));

	//MODELS
	pointer_model = CaveLib::loadModel("data/CellStroll/models/sphere.obj", new ModelLoadOptions(1.0f));
	cell_model = CaveLib::loadModel("data/CellStroll/models/AnimallCell.obj", new ModelLoadOptions(10.0f));
	cube_model = CaveLib::loadModel("data/CellStroll/models/cube.obj", new ModelLoadOptions(100.0f));

	//SHADERS
	simpleShader = new ShaderProgram("data/CellStroll/shaders/simple.vert", "data/CellStroll/shaders/simple.frag");
	simpleShader->link();

	cellShader = new ShaderProgram("data/CellStroll/shaders/cell.vert", "data/CellStroll/shaders/cell.frag");
	cellShader->link();

	airShader = new ShaderProgram("data/CellStroll/shaders/air.vert", "data/CellStroll/shaders/air.frag");
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

	if (GetAsyncKeyState(VK_RIGHT))
	{
		clippingPlane.point.y += 0.25f;
		std::cout << clippingPlane.point.y << "\n";
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		clippingPlane.point.y -= 0.25f;
		std::cout << clippingPlane.point.y << "\n";
	}
}

void CellStrollApp::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	// CONFIGS
	glEnable(GL_COLOR_MATERIAL);

	// UPDATE TIME UNIFORM
	GLfloat time = GLfloat(clock()) / GLfloat(CLOCKS_PER_SEC);

	// CLIPPING PLANE
	//glm::vec3 cPlane = glm::cross(clippingPlane.normal, clippingPlane.point);
	glm::vec3 cPlane = rescaledPalmPosition(leapData.palmPosition);

	// MVP
	glm::mat4 mvp = projectionMatrix * modelViewMatrix;
	glm::mat4 pointerMvp = glm::translate(mvp, rescaledPalmPosition(leapData.palmPosition));
	glm::mat4 airMvp = glm::translate(mvp, glm::vec3(-50.0f, -50.0f, 50.0f));

	// CELL ORIENTATION
	glm::mat4 cellMm = glm::mat4();
	glm::mat4 cellMvm = modelViewMatrix * cellMm;

	// DRAW POINTER
	simpleShader->use();
	glDisable(GL_TEXTURE_2D);
	simpleShader->setUniformMatrix4("modelViewMatrix", modelViewMatrix);
	simpleShader->setUniformVec3("light.position", pointLight.position);
	simpleShader->setUniformVec3("light.intensities", pointLight.intensities);
	simpleShader->setUniformFloat("light.attenuation", pointLight.attentuation);
	simpleShader->setUniformFloat("light.ambientCoefficient", pointLight.ambientCoefficient);
	simpleShader->setUniformFloat("time", time);
	simpleShader->setUniformVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
	simpleShader->setUniformVec3("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	simpleShader->setUniformFloat("materialShininess", 5.0f);
	simpleShader->setUniformVec3("cameraPosition", extractCameraPosition(simCamera.getData()));
	simpleShader->setUniformMatrix4("modelViewProjectionMatrix", pointerMvp);
	pointer_model->draw(simpleShader);

	// DRAW CELL
	cellShader->use();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, normalmap_a->tid());
	cellShader->setUniformInt("s_texture", 0);
	cellShader->setUniformMatrix4("modelMatrix", cellMm);
	cellShader->setUniformMatrix4("modelViewMatrix", cellMvm);
	cellShader->setUniformMatrix4("projectionMatrix", projectionMatrix);
	cellShader->setUniformVec4("clippingPlane", glm::vec4(cPlane, 0.0f));
	cellShader->setUniformVec3("light.position", pointLight.position);
	cellShader->setUniformVec3("light.intensities", pointLight.intensities);
	cellShader->setUniformFloat("light.attenuation", pointLight.attentuation);
	cellShader->setUniformFloat("light.ambientCoefficient", pointLight.ambientCoefficient);
	cellShader->setUniformFloat("time", time);
	cellShader->setUniformVec3("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	cellShader->setUniformFloat("materialShininess", 5.0f);
	cellShader->setUniformVec3("cameraPosition", extractCameraPosition(simCamera.getData()));
	cell_model->draw(cellShader);

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

glm::vec3 CellStrollApp::rescaledPalmPosition(glm::vec3 palmPos)
{
	return glm::vec3((palmPos.x / 4), (palmPos.y / 8) - 20.0f, (palmPos.z / 4));
}