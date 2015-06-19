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

		inline glm::vec3 getPointer()
		{
			return glm::vec3(
				rescale(palmPosition).x + direction.x * 5,
				rescale(palmPosition).y + direction.y * 5,
				rescale(palmPosition).z + direction.z * 5);
		}

		inline static glm::vec3 rescale(glm::vec3 v)
		{
			return glm::vec3((v.x / 16), (v.y / 16) - 10.0f, (v.z / 16));
		}
};