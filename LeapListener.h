#pragma once
#include "Leap.h"
#include <GL\glew.h>
#include <ctime>

class LeapListener : public Leap::Listener
{
	private:
		GLfloat lastLeapUpdate = 0, timeDiff = 0;
		const int LEAP_UPDATES_PER_SEC = 10;

	public:
		LeapListener();
		~LeapListener();

		virtual void onInit(const Leap::Controller &);
		virtual void onFrame(const Leap::Controller &);
		void LeapListener::setTimeDiff(GLfloat);
};