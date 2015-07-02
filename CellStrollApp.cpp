#pragma comment(lib,"opengl32")
#include "CellStrollApp.h"
#define _USE_MATH_DEFINES

CellStrollApp::CellStrollApp(void){}
CellStrollApp::~CellStrollApp(void){}

void CellStrollApp::init(void)
{
	//CONFIGS
	setPositionalDevice(SIMULATION_VIEW);
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

    // Method i was working on in order to read data from a json file into an array
	//fillInformationArray();

	//LEAP
	controller.addListener(leapListener);
	leapListener.setLeapData(&leapData);
	leapListener.onInit(controller);
	leapData.handDifference = -2;

	//CELL ELEMENT ARRAY
	std::string eNames[] { "centriole", "core", "flagellum", "golgi", "liquidlayer", "lysosome",
		"middleCore", "mitochondrion", "outerCore", "outerLayer", "peroxisome", "reticulum", "bezier" };
	for (int i = 0; i < MAX_CELL_ELEMENTS; i++)
		elementNames[i] = eNames[i];

	//TEXTURES/MODELS/SHADERS
	loadTextures();
	loadModels();
	loadShaders();

	//FBO
	glGenTextures(1, &fbo.texID);
	glBindTexture(GL_TEXTURE_2D, fbo.texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenSize.x, screenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &fbo.byteDataTexID);
	glBindTexture(GL_TEXTURE_2D, fbo.byteDataTexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenSize.x, screenSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &fbo.rboID);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo.rboID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenSize.x, screenSize.y);

	glGenFramebuffers(1, &fbo.fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboID);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.texID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, fbo.byteDataTexID, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo.rboID);

	fb_status("x");

	partLabel.height = 0.5;
	partLabel.width = 1;
	partLabel.setFont(font);

	// code to write the information in the json file
	std::ifstream cellPartFile("data/CellStroll/Json/cellParts.json");
	std::ostringstream tmp;
	tmp << cellPartFile.rdbuf();
	std::string s = tmp.str();
	std::cout << s << std::endl;
}

void CellStrollApp::displayInformationCellPart(int part)
{
	switch (part)
	{
	case 0:
		drawText("Centriole / Centriolen", 22, 900, 600, 0);
		break;
	case 1:
		drawText("Nucleolos / Kernlichaam", 23, 900, 600, 0);
		break;
	case 2:
		drawText("Flagellum / Flagellum", 21, 900, 600, 0);
		break;
	case 3:
		drawText("Golgi / Golgi-apparaat", 22, 900, 600, 0);
		break;
	case 4:
		drawText("Cytoplasm / Cytoplasma", 22, 900, 600, 0);
		break;
	case 5:
		drawText("Lysosome / Lysosoom", 19, 900, 600, 0);
		break;
	case 6:
		drawText("Nucleus / Celkern", 17, 900, 600, 0);
		break;
	case 7:
		drawText("Mitochondrion / Mitochondriën", 29, 900, 600, 0);
		break;
	case 8:
		drawText("Nuclear Envelope / Nuclear Envelope", 35, 800, 600, 0);
		break;
	case 9:
		drawText("Cell Membrane / Cel Membraan", 28, 900, 600, 0);
		break;
	case 10:
		drawText("Peroxisome / Peroxisoom", 23, 900, 600, 0);
		break;
	case 11:
		drawText("Endoplasmic Reticulum / Endoplasmatisch Reticulum", 49, 700, 600, 0);
		break;
	case 12:
		drawText("Filament / Filament", 19, 900, 600, 0);
		break;
	default:
		break;
	}
}

void CellStrollApp::fillInformationArray()
{
	int bracketCounter = 0;
	int accoladeCounter = 0;
	char *tempName, *tempDescription, *tempNl;
	Jzon::Object rootNode;
	Jzon::FileReader::ReadFile("data/CellStroll/Json/cellParts.json", rootNode);
	
	for (Jzon::Object::iterator it = rootNode.begin(); it != rootNode.end(); ++it)
	{
		std::string name = (*it).first;
		Jzon::Node &node = (*it).second;

		std::cout << name << " = ";
		if (node.IsValue())
		{
			if (name == "name")
			{
				for each (char c in node.ToString())
				{
					tempName += c;
				}
			}
			else if (name == "description")
			{
				for each (char c in node.ToString())
				{
					tempDescription += c;
				}
			}
			else if (name == "nl_name")
			{
				for each (char c in node.ToString())
				{
					tempNl += c;
				}
			}

			std::cout << node.ToString();
		}
		std::cout << std::endl;
	}
}

void CellStrollApp::preFrame(double, double totalTime)
{
	GLfloat timeFctr = GLfloat(clock() - clock_start) / CLOCKS_PER_SEC; // calculate time(s) elapsed since last frame
	clock_start = clock();
	leapListener.setTimeDiff(timeFctr);
}

// Draw text in the 3 dimensional space
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

// Draw text on the screen of the application
void CellStrollApp::drawText(const char *text, int length, int x, int y, int z) {
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glColor3f(248/255, 248/255, 255/255);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1280, 0, 720, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glLoadIdentity();
	glRasterPos3i(x, y, z);
	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);
		//glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void CellStrollApp::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix)
{
	// CONFIGS
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);

	// UPDATES
	GLfloat time = GLfloat(clock()) / GLfloat(CLOCKS_PER_SEC);
	GLfloat line[6];
	bool isPointing = false;

	// MVP
	glm::mat4 mvp = projectionMatrix * modelViewMatrix;
	glm::mat4 pointerMvp = glm::translate(mvp, LeapData::rescale(leapData.palmPosition));
	glm::mat4 pointerDirectionMvp;
	glm::mat4 cellMm = glm::translate(glm::mat4(), center);
	glm::mat4 punaiseMm = glm::translate(glm::mat4(), glm::vec3(-250, 0, 0));

	// GESTURES
	glm::mat4 rotMatNormal = glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), leapData.palmNormal);
	glm::mat4 rotMatDir = glm::orientation(glm::vec3(0.0f, 0.0f, -1.0f), leapData.direction);
	pointerMvp *= glm::inverse(rotMatDir * rotMatNormal);

	if (leapListener.getHandMode() == LeapListener::HANDMODE_SLICE)
	{
		cellMm *= glm::orientation(glm::vec3(0.0f, -1.0f, 0.0f), glm::normalize(leapData.tempPalmPosition)); // Waarom -1.0f op de y-as?
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
		isPointing = true;
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

	// SCALE PUNAISE
	punaiseMm = glm::scale(punaiseMm, glm::vec3(250.0f));
	punaiseMm = glm::scale(punaiseMm, glm::vec3(cellScale));
	glm::mat4 punaiseMvm = modelViewMatrix * punaiseMm;

	GLint oldFbo;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);

	// DRAWBUFFERS
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboID);
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, buffers);
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
	cellShader->setUniformMatrix4("modelMatrix", cellMm);
	cellShader->setUniformMatrix4("modelViewMatrix", cellMvm);
	cellShader->setUniformMatrix4("projectionMatrix", projectionMatrix);
	cellShader->setUniformVec3("pointerPosition", leapData.getPointer());
	cellShader->setUniformVec3("clippingPlane.point", clippingPlane.point);
	cellShader->setUniformVec3("clippingPlane.normal", clippingPlane.normal);
	cellShader->setUniformVec3("light.position", pointLight.position);
	cellShader->setUniformVec3("light.intensities", pointLight.intensities);
	cellShader->setUniformFloat("light.attenuation", pointLight.attentuation);
	cellShader->setUniformFloat("light.ambientCoefficient", pointLight.ambientCoefficient);
	cellShader->setUniformFloat("time", time);
	cellShader->setUniformBool("isPointing", isPointing);
	cellShader->setUniformVec3("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	cellShader->setUniformFloat("materialShininess", 5.0f);
	cellShader->setUniformVec3("cameraPosition", extractCameraPosition(positionalDeviceCamera.getData()));
	for (int i = 0; i < MAX_CELL_ELEMENTS; i++)
	{
		if (selected && i == selectionIndex)
			cellShader->setUniformBool("selected", true);
		else
			cellShader->setUniformBool("selected", false);
		cellShader->setUniformInt("elementIndex", i);
		cellElements[i]->draw(cellShader);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//DRAW PUNAISE
	punaiseShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, punaiseTexture->tid());
	punaiseShader->setUniformInt("s_texture", 0);
	punaiseShader->setUniformMatrix4("modelMatrix", punaiseMm);
	punaiseShader->setUniformMatrix4("modelViewMatrix", punaiseMvm);
	punaiseShader->setUniformMatrix4("projectionMatrix", projectionMatrix);
	punaiseShader->setUniformVec3("light.position", pointLight.position);
	punaiseShader->setUniformVec3("light.intensities", pointLight.intensities);
	punaiseShader->setUniformFloat("light.attenuation", pointLight.attentuation);
	punaiseShader->setUniformFloat("light.ambientCoefficient", pointLight.ambientCoefficient);
	punaiseShader->setUniformVec3("materialSpecularColor", glm::vec3(1.0f, 1.0f, 1.0f));
	punaiseShader->setUniformFloat("materialShininess", 5.0f);
	punaiseShader->setUniformVec3("cameraPosition", extractCameraPosition(positionalDeviceCamera.getData()));
	punaise_model->draw(punaiseShader);
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
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

	std::vector<glm::vec2> verts;
	verts.push_back(glm::vec2(-1, -1));
	verts.push_back(glm::vec2(1, -1));
	verts.push_back(glm::vec2(1, 1));
	verts.push_back(glm::vec2(-1, 1));

	// DRAW TO DEFAULT FRAME BUFFER
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	fboShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbo.texID);
	fboShader->setUniformInt("screenTexture", 0);
	glBindVertexArray(0);
	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 2 * 4, &verts[0]);
	glDrawArrays(GL_QUADS, 0, verts.size());

	glDisableVertexAttribArray(0);
	glUseProgram(0);

	//READ BYTEDATA
	if (leapListener.getHandMode() == LeapListener::HANDMODE_FINGER)
	{
		glBindTexture(GL_TEXTURE_2D, fbo.byteDataTexID);
		GLint texSize = screenSize.x * screenSize.y;
		GLubyte* pixels = new GLubyte[texSize];
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);

		int j = 0;
		while (j <= texSize)
		{
			if (pixels[j] != 0 && pixels[j] != 255)
				break;
			j ++;
		}

		if (j < texSize)
		{
			int value = pixels[j];
			selectionIndex = value;
			selected = true;
		}
		else
			selected = false;

		displayInformationCellPart(selectionIndex);
	}
}

void CellStrollApp::loadTextures()
{
	cellTexture = CaveLib::loadTexture("data/CellStroll/models/CoreTextureNew.png", new TextureLoadOptions(GL_FALSE));
	sliceTexture = CaveLib::loadTexture("data/CellStroll/models/hand.png", new TextureLoadOptions(GL_FALSE));
	fingerTexture = CaveLib::loadTexture("data/CellStroll/models/finger.png", new TextureLoadOptions(GL_FALSE));
	fistTexture = CaveLib::loadTexture("data/CellStroll/models/fist.png", new TextureLoadOptions(GL_FALSE));
	punaiseTexture = CaveLib::loadTexture("data/CellStroll/models/punaiseTexture.png", new TextureLoadOptions(GL_FALSE));
	normalmap_a = CaveLib::loadTexture("data/CellStroll/textures/normalmap3.png", new TextureLoadOptions(GL_FALSE));
	handTexture = sliceTexture;
}

void CellStrollApp::loadModels()
{
	// CELL
	for (int i = 0; i < MAX_CELL_ELEMENTS; i++)
	{
		std::stringstream path;
		path << "data/CellStroll/models/AnimalCell/" << elementNames[i] << ".obj";
		cellElements[i] = CaveLib::loadModel(path.str());
	}

	//ALL ELSE
	hand_model = CaveLib::loadModel("data/CellStroll/models/hand.obj", new ModelLoadOptions(3.0f));
	printf("Loaded hand\n");
	air_model = CaveLib::loadModel("data/CellStroll/models/sphere2.obj", new ModelLoadOptions(1500.0f));
	printf("Loaded surroundings\n");
	pointer_model = CaveLib::loadModel("data/CellStroll/models/sphere.obj", new ModelLoadOptions(0.25f));
	printf("Loaded pointer\n");
	punaise_model = CaveLib::loadModel("data/CellStroll/models/punaise.obj");
	printf("Loaded punaise model\n");
}

void CellStrollApp::loadShaders()
{
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
	punaiseShader = new ShaderProgram("data/CellStroll/shaders/punaise.vert", "data/CellStroll/shaders/punaise.frag");
	punaiseShader->link();
	lineShader = createShaderProgram("data/CellStroll/shaders/line.vert", "data/CellStroll/shaders/line.frag");
	glLinkProgram(lineShader);
}

void CellStrollApp::setPositionalDevice(ViewConfig config)
{
	viewConfig = config;
	if (config == OCULUS_VIEW)
		positionalDeviceCamera.init("MainUserHead");
	else
		positionalDeviceCamera.init("CameraPosition");
}