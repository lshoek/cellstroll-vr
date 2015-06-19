#pragma once
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>

class LeapData
{
	public:
		inline LeapData(){};
		inline ~LeapData(){};

		LeapData* ptr = this;
		glm::vec3 palmPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 tempPalmPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 palmNormal = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 direction = glm::vec3(0.0f, 0.0f, 1.0f);
		bool isRight;
		float pitch, roll, yaw, handDifference;
};