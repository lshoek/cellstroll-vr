#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#pragma comment(lib, "glew32.lib")

class Shader
{
	public:
		GLuint programId;
		GLuint modelViewUniform;

		Shader(GLchar* vsfile, GLchar* fsfile);
		~Shader();

		void checkShaderErrors(GLuint);
		void link();
		void use();
		void setUniform();
		void bindAttribute();
};