#include <vector>
#include <fstream>
#include <string>
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>

inline GLuint loadShader(std::string fname, GLenum type)
{
	std::string data = "";
	std::string line;
	std::ifstream file(fname);
	while (!file.eof() && file.good())
	{
		std::getline(file, line);
		data += line + "\n";
	}

	GLuint shaderId = glCreateShader(type);
	const char* d2 = data.c_str();
	glShaderSource(shaderId, 1, &d2, NULL);
	return shaderId;
}

inline GLuint createShaderProgram(std::string vertexShader, std::string fragmentShader) 
{
	GLint test;
	GLuint vShader = loadShader(vertexShader, GL_VERTEX_SHADER);
	GLuint fShader = loadShader(fragmentShader, GL_FRAGMENT_SHADER);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &test);
	if (!test)
	{
		std::vector<char> compilation_log(512);
		glGetShaderInfoLog(vShader, compilation_log.size(), NULL, &compilation_log[0]);
		std::cerr << &compilation_log[0] << std::endl;
	}
	glCompileShader(fShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &test);
	if (!test)
	{
		std::vector<char> compilation_log(512);
		glGetShaderInfoLog(fShader, compilation_log.size(), NULL, &compilation_log[0]);
		std::cerr << &compilation_log[0] << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	
	return shaderProgram;
}

inline bool sphereCollision(glm::vec3 pointer, glm::vec3 sphereCenter, GLfloat sphereRadius)
{
	glm::vec3 diff = pointer - sphereCenter;
	GLfloat distance = glm::sqrt(diff.x * diff.x + diff.y * diff.y + diff.z*diff.z);
	if (distance < sphereRadius)
		return true;
	return false;
}

inline glm::vec3 extractCameraPosition(const glm::mat4 &modelView)
{
	glm::mat3 rotMat = glm::mat3(modelView);
	glm::vec3 d(modelView[3]);
	return -d * rotMat;
}

static void fb_status(const char *where)
{
	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
	case GL_FRAMEBUFFER_COMPLETE:
		printf(" :: GL_FRAMEBUFFER_COMPLETE in %s\n", where);
		break;

	case GL_FRAMEBUFFER_UNDEFINED:
		printf(" :: GL_FRAMEBUFFER_UNDEFINED in %s\n", where);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		printf(" :: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT in %s\n", where);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		printf(" :: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT in %s\n", where);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		printf(" :: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER in %s\n", where);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		printf(" :: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER in %s\n", where);
		break;

	case GL_FRAMEBUFFER_UNSUPPORTED:
		printf(" :: GL_FRAMEBUFFER_UNSUPPORTED in %s\n", where);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		printf(" :: GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE in %s\n", where);
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		printf(" :: GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS in %s\n", where);
		break;
	}
}