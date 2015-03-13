#include <iostream>
#include <fstream>
#include "Shader.h"

#ifdef WIN32
void GLAPIENTRY onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
#else
void onDebug(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam)
#endif
{
	std::cout << message << std::endl;
}

Shader::Shader(GLchar* vsfile, GLchar* fsfile)
{
	std::ifstream vertexShaderFile(vsfile);
	std::string vertexShaderData((std::istreambuf_iterator<char>(vertexShaderFile)), std::istreambuf_iterator<char>());
	const GLchar* cvertexShaderData = vertexShaderData.c_str();

	std::ifstream fragShaderFile(fsfile);
	std::string fragShaderData((std::istreambuf_iterator<char>(fragShaderFile)), std::istreambuf_iterator<char>());
	const GLchar* cfragShaderData = fragShaderData.c_str();

	programId = glCreateProgram();							// maak een shaderprogramma aan

	GLuint vertexId = glCreateShader(GL_VERTEX_SHADER);		// maak vertex shader aan
	glShaderSource(vertexId, 1, &cvertexShaderData, NULL);	// laat opengl de shader uit de variabele 'vertexShader' halen
	glCompileShader(vertexId);								// compileer de shader
	checkShaderErrors(vertexId);							// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(programId, vertexId);					// hang de shader aan het shaderprogramma

	GLuint fragmentId = glCreateShader(GL_FRAGMENT_SHADER);	// maak fragment shader aan
	glShaderSource(fragmentId, 1, &cfragShaderData, NULL);	// laat opengl de shader uit de variabele 'fragmentShader' halen
	glCompileShader(fragmentId);							// compileer de shader
	checkShaderErrors(fragmentId);							// controleer of er fouten zijn opgetreden bij het compileren
	glAttachShader(programId, fragmentId);					// hang de shader aan het shaderprogramma

	glBindAttribLocation(programId, 0, "a_position");		// zet de positie op vertex attribuut 0
	glBindAttribLocation(programId, 1, "a_color");			// zet de kleur op vertex attribuut 1
	glLinkProgram(programId);								// link het programma, zorg dat alle attributes en varying gelinked zijn
	glUseProgram(programId);								// Zet dit als actieve programma

	modelViewUniform = glGetUniformLocation(programId, "modelViewProjectionMatrix");	//haal de uniform van modelViewMatrix op

	glEnableVertexAttribArray(0);							// we gebruiken vertex attribute 0
	glEnableVertexAttribArray(1);							// en vertex attribute 1

	if (glDebugMessageCallback)
	{
		glDebugMessageCallback(&onDebug, NULL);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
		glEnable(GL_DEBUG_OUTPUT);
	}
}

void Shader::checkShaderErrors(GLuint shaderId)
{
	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);					//kijk of het compileren is gelukt
	if (status == GL_FALSE)
	{
		int length, charsWritten;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);				//haal de lengte van de foutmelding op
		char* infolog = new char[length + 1];
		memset(infolog, 0, length + 1);
		glGetShaderInfoLog(shaderId, length, &charsWritten, infolog);		//en haal de foutmelding zelf op
		std::cout << "Error compiling shader:\n" << infolog << std::endl;
		delete[] infolog;
	}
}