#pragma comment(lib,"opengl32")
#include "CellStrollApp.h"
#define _USE_MATH_DEFINES

CellStrollApp::CellStrollApp(void){}
CellStrollApp::~CellStrollApp(void){}

void CellStrollApp::init(void)
{
	//CONFIGS
	setPositionalDevice(CellStrollApp::OCULUS_VIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// SCREEN & WORLD
	screenSize = glm::vec2(Kernel::getInstance()->getWindowWidth(), Kernel::getInstance()->getWindowHeight());
	center = glm::vec3(0.0f, 0.0f, -10.0f);

	//LIGHTING
	pointLight.position = glm::vec3(20.0f, 10.0f, 0.0f);
	pointLight.intensities = glm::vec3(1.0f, 1.0f, 1.0f);
	pointLight.ambientCoefficient = 0.75f;
	pointLight.attentuation = 0.2f;

	//CLIPPING PLANE
	clippingPlane.point = glm::vec3(100.0f, 0.0f, 0.0f);
	clippingPlane.normal = glm::vec3(0.0f, -1.0f, 0.0f);

	//TEXTURES
	cellTexture = CaveLib::loadTexture("data/CellStroll/models/CoreTextureNew.png", new TextureLoadOptions(GL_FALSE));
	sliceTexture = CaveLib::loadTexture("data/CellStroll/models/hand.png", new TextureLoadOptions(GL_FALSE));
	fingerTexture = CaveLib::loadTexture("data/CellStroll/models/finger.png", new TextureLoadOptions(GL_FALSE));
	fistTexture = CaveLib::loadTexture("data/CellStroll/models/fist.png", new TextureLoadOptions(GL_FALSE));
	normalmap_a = CaveLib::loadTexture("data/CellStroll/textures/normalmap3.png", new TextureLoadOptions(GL_FALSE));
	handTexture = sliceTexture;

	//MODELS
	cell_model = CaveLib::loadModel("data/CellStroll/models/AnimallCellNew.obj", new ModelLoadOptions(10.0f));
	hand_model = CaveLib::loadModel("data/CellStroll/models/hand.obj", new ModelLoadOptions(3.0f));
	air_model = CaveLib::loadModel("data/CellStroll/models/sphere2.obj", new ModelLoadOptions(100.0f));
	pointer_model = CaveLib::loadModel("data/CellStroll/models/sphere.obj", new ModelLoadOptions(0.25f));
	printf("vertices cell: %f %f %f", cell_model->getVertices()[0], cell_model->getVertices()[1], cell_model->getVertices()[2]);

	//SHADERS
	handShader = new ShaderProgram("data/CellStroll/shaders/hand.vert", "data/CellStroll/shaders/hand.frag");
	handShader->link();

	pointerShader = new ShaderProgram("data/CellStroll/shaders/pointer.vert", "data/CellStroll/shaders/pointer.frag");
	pointerShader->link();

	cellShader = new ShaderProgram("data/CellStroll/shaders/cell.vert", "data/CellStroll/shaders/cell.frag");
	cellShader->link();

	airShader = new ShaderProgram("data/CellStroll/shaders/air.vert", "data/CellStroll/shaders/air.frag");
	airShader->link();

	fboShader = new ShaderProgram("data/CellStroll/shaders/fbo.vert", "data/CellStroll/shaders/fbo.frag");
	fboShader->link();

	lineShader = createShaderProgram("data/CellStroll/shaders/line.vert", "data/CellStroll/shaders/line.frag");
	glLinkProgram(lineShader);

	//LEAP
	controller.addListener(leapListener);
	leapListener.setLeapData(&leapData);
	leapListener.onInit(controller);
	leapData.handDifference = -2;

	//FBO
	glGenTextures(1, &fbo.texID);
	glGenTextures(1, &fbo.byteDataTexID);
	glBindTexture(GL_TEXTURE_2D, fbo.texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenSize.x, screenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, fbo.byteDataTexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenSize.x, screenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &fbo.rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo.rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo.rboID);

	glGenFramebuffers(1, &fbo.fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboID);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.texID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fbo.byteDataTexID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo.rboID);
	fb_status("x");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	cFont* font = new cFont("Tahoma");
	partLabel.text = string;
	partLabel.height = 1;
	partLabel.width = 2;
	partLabel.setFont(font);
	//glTranslatef(x, y, 0);
	partLabel.x = x;
	partLabel.y = y;
	partLabel.render(0.0f);
	//partLabel.drawBox(x, y, 0, 2, 1, 1);
}

void CellStrollApp::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	// CONFIGS
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);

	// UPDATES
	GLfloat time = GLfloat(clock()) / GLfloat(CLOCKS_PER_SEC);
	GLfloat line[6];
	bool collision = false;

	// MVP
	glm::mat4 mvp = projectionMatrix * modelViewMatrix;
	glm::mat4 pointerMvp = glm::translate(mvp, LeapData::rescale(leapData.palmPosition));
	glm::mat4 pointerDirectionMvp;
	glm::mat4 cellMm = glm::translate(glm::mat4(), center);

	// GESTURES
	glm::mat4 rotMatNormal = glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), leapData.palmNormal);
	glm::mat4 rotMatDir = glm::orientation(glm::vec3(0.0f, 0.0f, -1.0f), leapData.direction);
	pointerMvp *= glm::inverse(rotMatDir * rotMatNormal);

	if (leapListener.getHandMode() == LeapListener::HANDMODE_SLICE)
	{
		cellMm *= glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), glm::normalize(leapData.tempPalmPosition));
		clippingPlane.point = LeapData::rescale(leapData.palmPosition);
		clippingPlane.normal = leapData.palmNormal;
		handTexture = sliceTexture;
	}
	else if (leapListener.getHandMode() == LeapListener::HANDMODE_FINGER)
	{
		cellMm *= glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), glm::normalize(leapData.tempPalmPosition));

		// DRAW POINTER LINE
		line[0] = LeapData::rescale(leapData.palmPosition).x;
		line[1] = LeapData::rescale(leapData.palmPosition).y;
		line[2] = LeapData::rescale(leapData.palmPosition).z;		
		line[3] = leapData.getPointer().x;
		line[4] = leapData.getPointer().y;
		line[5] = leapData.getPointer().z;

		pointerDirectionMvp *= glm::translate(mvp, leapData.getPointer());
		collision = sphereCollision(leapData.getPointer(), center, 4.0f);
		handTexture = fingerTexture;
	}
	else if (leapListener.getHandMode() == LeapListener::HANDMODE_FIST)
	{
		cellMm *= glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), glm::normalize(leapData.palmPosition));
		leapData.ptr->tempPalmPosition = leapData.palmPosition;
		handTexture = fistTexture;
	}
	else if (leapListener.getHandMode() == LeapListener::HANDMODE_ZOOM)
	{
		if (leapData.handDifference / 50 - 2 >= 0 && leapData.handDifference / 50 - 2 <= 8)
			cellScale = (leapData.handDifference / 50 - 2);
	}

	// SCALE CELL
	cellMm = glm::scale(cellMm, glm::vec3(cellScale));
	glm::mat4 cellMvm = modelViewMatrix * cellMm;

	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboID);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// DRAW HAND
	handShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, handTexture->tid());
	handShader->setUniformInt("s_texture", 0);
	handShader->setUniformMatrix4("modelViewProjectionMatrix", pointerMvp);
	handShader->setUniformBool("isRight", leapData.isRight);
	hand_model->draw(handShader);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// DRAW CELL
	cellShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cellTexture->tid());
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
	cellShader->setUniformVec3("cameraPosition", extractCameraPosition(positionalDeviceCamera.getData()));
	cell_model->draw(cellShader);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// DRAW AIR
	airShader->use();
	airShader->setUniformFloat("time", time);
	airShader->setUniformMatrix4("modelViewProjectionMatrix", glm::rotate(mvp, 90.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
	air_model->draw(airShader);

	if (leapListener.getHandMode() == LeapListener::HANDMODE_FINGER)
	{
		// DRAW POINTER
		pointerShader->use();
		pointerShader->setUniformMatrix4("modelViewProjectionMatrix", pointerDirectionMvp);
		pointer_model->draw(pointerShader);

		/*
		// DRAWING LINE BETWEEN HAND AND POINTER
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
		*/
	}

	//FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(-1, -1));
	verts.push_back(glm::vec2(1, -1));
	verts.push_back(glm::vec2(1, 1));
	verts.push_back(glm::vec2(-1, 1));

	fboShader->use();
	fboShader->setUniformFloat("time", time);
	fboShader->setUniformVec2("screenSize", screenSize);

	glBindVertexArray(0);
	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindTexture(GL_TEXTURE_2D, fbo.texID);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * 4, &verts[0]);
	glDrawArrays(GL_QUADS, 0, verts.size());

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

void CellStrollApp::setPositionalDevice(ViewConfig config)
{
	if (config == OCULUS_VIEW)
		positionalDeviceCamera.init("MainUserHead");
	else
		positionalDeviceCamera.init("CameraPosition");
}

glm::vec3 CellStrollApp::extractCameraPosition(const glm::mat4 &modelView)
{
	glm::mat3 rotMat = glm::mat3(modelView);
	glm::vec3 d(modelView[3]);
	return -d * rotMat;
}