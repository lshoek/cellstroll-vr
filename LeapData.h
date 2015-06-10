#pragma once
#include <GL\glew.h>
#include <glm/gtc/type_ptr.hpp>

class LeapData
{
	public:
		LeapData();
		~LeapData();

		glm::vec3 palmPosition, palmNormal;
		bool isRight;
		float pitch, roll, yaw, handDifference;
};