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
	std::cout << distance << "\n";
	if (distance < sphereRadius)
		return true;
	return false;
}