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
	cellTexture = CaveLib::loadTexture("data/CellStroll/models/CoreTextureNew.png", new TextureLoadOptions(GL_FALSE));
	sliceTexture = CaveLib::loadTexture("data/CellStroll/models/hand.png", new TextureLoadOptions(GL_FALSE));
	fingerTexture = CaveLib::loadTexture("data/CellStroll/models/finger.png", new TextureLoadOptions(GL_FALSE));
	fistTexture = CaveLib::loadTexture("data/CellStroll/models/fist.png", new TextureLoadOptions(GL_FALSE));
	normalmap_a = CaveLib::loadTexture("data/CellStroll/textures/normalmap3.png", new TextureLoadOptions(GL_FALSE));
	handTexture = sliceTexture;

	//MODELS
	// Animall models
	centriole_model			= CaveLib::loadModel("data/CellStroll/models/AnimalCell/centriole.obj");
	printf("Loaded centriole model\n");
	nucleolos_model			= CaveLib::loadModel("data/CellStroll/models/AnimalCell/core.obj");
	printf("Loaded nucleolos model\n");
	flagellum_model			= CaveLib::loadModel("data/CellStroll/models/AnimalCell/flagellum.obj");
	printf("Loaded flagellum model\n");
	golgi_model				= CaveLib::loadModel("data/CellStroll/models/AnimalCell/golgi.obj");
	printf("Loaded golgi model\n");
	cytoplasm_model			= CaveLib::loadModel("data/CellStroll/models/AnimalCell/liquidLayer.obj");
	printf("Loaded cytoplasm model\n");
	lysosome_model			= CaveLib::loadModel("data/CellStroll/models/AnimalCell/lysosome.obj");
	printf("Loaded lysosome model\n");
	nucleus_model			= CaveLib::loadModel("data/CellStroll/models/AnimalCell/middleCore.obj");
	printf("Loaded nucleus model\n");
	mitochondrion_model		= CaveLib::loadModel("data/CellStroll/models/AnimalCell/mitochondrion.obj");
	printf("Loaded mitochondrion model\n");
	nuclearMembrane_model	= CaveLib::loadModel("data/CellStroll/models/AnimalCell/outerCore.obj");
	printf("Loaded nuclear membrane model\n");
	cellMembrane_model		= CaveLib::loadModel("data/CellStroll/models/AnimalCell/outerLayer.obj");
	printf("Loaded cell membrane model\n");
	peroxisome_model		= CaveLib::loadModel("data/CellStroll/models/AnimalCell/peroxisome.obj");
	printf("Loaded peroxisome model\n");
	reticulum_model			= CaveLib::loadModel("data/CellStroll/models/AnimalCell/reticulum.obj");
	printf("Loaded reticulum model\n");
	filament_model			= CaveLib::loadModel("data/CellStroll/models/AnimalCell/bezier.obj");
	printf("Loaded filament model\n");

	// Other models
	//cell_model = CaveLib::loadModel("data/CellStroll/models/animallcellnew.obj", new ModelLoadOptions(10.0f));
	hand_model = CaveLib::loadModel("data/CellStroll/models/hand.obj", new ModelLoadOptions(3.0f));
	printf("Loaded hand\n");
	cube_model = CaveLib::loadModel("data/CellStroll/models/cube.obj", new ModelLoadOptions(1000.0f));
	printf("Loaded surroundings\n");
	pointer_model = CaveLib::loadModel("data/CellStroll/models/sphere.obj", new ModelLoadOptions(0.25f));
	printf("Loaded pointer\n");
	punaise_model = CaveLib::loadModel("data/CellStroll/models/punaise.obj", new ModelLoadOptions(250.0f));
	printf("Loaded punaise model\n");
	//printf("vertices cell: %f %f %f", cell_model->getVertices()[0], cell_model->getVertices()[1], cell_model->getVertices()[2]);

	//SHADERS
	simpleShader = new ShaderProgram("data/CellStroll/shaders/simple.vert", "data/CellStroll/shaders/simple.frag");
	simpleShader->link();

	cellShader = new ShaderProgram("data/CellStroll/shaders/cell.vert", "data/CellStroll/shaders/cell.frag");
	cellShader->link();

	airShader = new ShaderProgram("data/CellStroll/shaders/air.vert", "data/CellStroll/shaders/air.frag");
	airShader->link();

	lineShader = createShaderProgram("data/CellStroll/shaders/line.vert", "data/CellStroll/shaders/line.frag");
	glLinkProgram(lineShader);

	//LEAP
	controller.addListener(leapListener);
	leapListener.setLeapData(&leapData);
	leapListener.onInit(controller);

	leapData.handDifference = -2;

	partLabel.height = 0.5;
	partLabel.width = 1;
	partLabel.setFont(font);
}

void CellStrollApp::preFrame(double, double totalTime)
{
	GLfloat timeFctr = GLfloat(clock() - clock_start) / CLOCKS_PER_SEC; // calculate time(s) elapsed since last frame
	clock_start = clock();
	leapListener.setTimeDiff(timeFctr);
}

// SHOWING TEXT ON THE SCREEN
void CellStrollApp::displayText(int x, int y, std::string string)
{
	glTranslatef(2, 0, 0);
	glRotatef(-45, 0, 1, 0);
	glTranslatef(0, -1, -1);
	partLabel.text = string;
	//glTranslatef(x, y, 0);
	partLabel.x = x;
	partLabel.y = y;
	partLabel.render(2.0f);
}

void CellStrollApp::drawBitmapText(std::string caption, int score, float r, float g, float b,float x, float y, float z) 
{
	glColor3f(r, g, b);
	glRasterPos3f(x, y, z);
	std::stringstream strm;
	strm << caption << score;
	std::string text = strm.str();
	for (std::string::iterator it = text.begin(); it != text.end(); ++it) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *it);
	}
}

void CellStrollApp::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	// CONFIGS
	glEnable(GL_COLOR_MATERIAL);

	// UPDATES
	GLfloat time = GLfloat(clock()) / GLfloat(CLOCKS_PER_SEC);
	GLfloat line[6];
	bool collision = false;

	// MVP
	glm::mat4 mvp = projectionMatrix * modelViewMatrix;
	glm::mat4 airMvp = glm::translate(mvp, glm::vec3(-50.0f, -50.0f, 50.0f));
	glm::mat4 pointerMvp = glm::translate(mvp, LeapData::rescale(leapData.palmPosition));
	glm::mat4 pointerDirectionMvp;
	glm::mat4 cellMm = glm::mat4();

	// GESTURES
	if (leapListener.getHandMode() == LeapListener::HANDMODE_SLICE)
	{
		glm::mat4 rotMat = glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), leapData.palmNormal);
		pointerMvp *= glm::inverse(rotMat);
		cellMm = glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), glm::normalize(leapData.tempPalmPosition));
		clippingPlane.point = LeapData::rescale(leapData.palmPosition);
		clippingPlane.normal = leapData.palmNormal;
		handTexture = sliceTexture;
	}
	else if (leapListener.getHandMode() == LeapListener::HANDMODE_FINGER)
	{
		glm::mat4 rotMat = glm::orientation(glm::vec3(0.0f, 0.0f, -1.0f), leapData.direction);
		rotMat *= glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), leapData.palmNormal);
		pointerMvp *= glm::inverse(rotMat);
		cellMm = glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), glm::normalize(leapData.tempPalmPosition));

		// DRAW POINTER LINE
		line[0] = LeapData::rescale(leapData.palmPosition).x;
		line[1] = LeapData::rescale(leapData.palmPosition).y;
		line[2] = LeapData::rescale(leapData.palmPosition).z;		
		line[3] = leapData.getPointer().x;
		line[4] = leapData.getPointer().y;
		line[5] = leapData.getPointer().z;

		pointerDirectionMvp = glm::translate(mvp, leapData.getPointer());
		collision = sphereCollision(leapData.getPointer(), glm::vec3(0.0f, 0.0f, 0.0f), 4.0f);
		handTexture = fingerTexture;

		std::string bleh = "Mooie tekst";
		displayText(1, 0, bleh);
	}
	else if (leapListener.getHandMode() == LeapListener::HANDMODE_FIST)
	{
		glm::mat4 rotMat = glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), leapData.palmNormal);
		pointerMvp *= glm::inverse(rotMat);
		cellMm = glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), glm::normalize(leapData.palmPosition));
		leapData.ptr->tempPalmPosition = leapData.palmPosition;
		handTexture = fistTexture;
	}
	else if (leapListener.getHandMode() == LeapListener::HANDMODE_ZOOM)
	{
		if (leapData.handDifference / 50 - 2 >= 0 && leapData.handDifference / 50 - 2 <= 8)
		{
			xScale = (leapData.handDifference / 50 - 2);
			yScale = (leapData.handDifference / 50 - 2);
			zScale = (leapData.handDifference / 50 - 2);
		}
	}

	// SCALING OF THE CELL
	cellMm = glm::scale(cellMm, glm::vec3(xScale, yScale, zScale));

	glm::mat4 cellMvm = modelViewMatrix * cellMm;

	// DRAW HAND
	simpleShader->use();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handTexture->tid());
	simpleShader->setUniformInt("s_texture", 0);
	simpleShader->setUniformMatrix4("modelViewProjectionMatrix", pointerMvp);
	simpleShader->setUniformVec3("light.position", pointLight.position);
	simpleShader->setUniformVec3("light.intensities", pointLight.intensities);
	simpleShader->setUniformFloat("light.attenuation", pointLight.attentuation);
	simpleShader->setUniformFloat("light.ambientCoefficient", pointLight.ambientCoefficient);
	simpleShader->setUniformFloat("time", time);
	simpleShader->setUniformBool("isRight", leapData.isRight);
	simpleShader->setUniformBool("texEnabled", true);
	simpleShader->setUniformVec3("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	simpleShader->setUniformFloat("materialShininess", 5.0f);
	simpleShader->setUniformVec3("cameraPosition", extractCameraPosition(simCamera.getData()));
	hand_model->draw(simpleShader);

	//DRAW PUNAISE
	// hier komen de instellingen voor het tekenen van de punaise

	// DRAW CELL
	cellShader->use();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cellTexture->tid());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalmap_a->tid());
	cellShader->setUniformInt("s_texture", 0);
	cellShader->setUniformInt("s_normals", 1);
	cellShader->setUniformMatrix4("modelMatrix", cellMm);
	cellShader->setUniformMatrix4("modelViewMatrix", cellMvm);
	cellShader->setUniformMatrix4("projectionMatrix", projectionMatrix);
	cellShader->setUniformVec3("clippingPlane.point", clippingPlane.point);
	cellShader->setUniformVec3("clippingPlane.normal", clippingPlane.normal);
	cellShader->setUniformVec3("light.position", pointLight.position);
	cellShader->setUniformVec3("light.intensities", pointLight.intensities);
	cellShader->setUniformFloat("light.attenuation", pointLight.attentuation);
	cellShader->setUniformFloat("light.ambientCoefficient", pointLight.ambientCoefficient);
	cellShader->setUniformFloat("time", time);
	cellShader->setUniformBool("collision", collision);
	cellShader->setUniformVec3("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	cellShader->setUniformFloat("materialShininess", 5.0f);
	cellShader->setUniformVec3("cameraPosition", extractCameraPosition(simCamera.getData()));
	centriole_model->		draw(cellShader);
	nucleolos_model->		draw(cellShader);
	flagellum_model->		draw(cellShader);
	golgi_model->			draw(cellShader);
	cytoplasm_model->		draw(cellShader);
	lysosome_model->		draw(cellShader);
	nucleus_model->			draw(cellShader);
	mitochondrion_model->	draw(cellShader);
	nuclearMembrane_model->	draw(cellShader);
	cellMembrane_model->	draw(cellShader);
	peroxisome_model->		draw(cellShader);
	reticulum_model->		draw(cellShader);
	filament_model->		draw(cellShader);

	// DRAW AIR
	airShader->use();
	airShader->setUniformFloat("time", time);
	airShader->setUniformMatrix4("modelViewProjectionMatrix", airMvp);
	cube_model->draw(airShader);

	// LINE /*
	if (leapListener.getHandMode() == LeapListener::HANDMODE_FINGER)
	{
		simpleShader->use();
		simpleShader->setUniformMatrix4("modelViewProjectionMatrix", pointerDirectionMvp);
		simpleShader->setUniformBool("texEnabled", false);
		pointer_model->draw(simpleShader);

		glUseProgram(lineShader);
		GLuint vertexArray, vertexBuffer;

		// VAO
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		//VBO
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

		// DRAW
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glUniformMatrix4fv(glGetUniformLocation(lineShader, "modelViewProjectionMatrix"), 1, 0, glm::value_ptr(mvp));

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glLineWidth(3.0f);
		glDrawArrays(GL_LINES, 0, 2);
		glDisableVertexAttribArray(0);
	}
	glUseProgram(0);
}

glm::vec3 CellStrollApp::extractCameraPosition(const glm::mat4 &modelView)
{
	glm::mat3 rotMat = glm::mat3(modelView);
	glm::vec3 d(modelView[3]);
	return -d * rotMat;
}