#pragma once
#include <GL\glew.h>
#include <ctime>
#include <glm\ext.hpp>
#include "Leap.h"
#include "LeapData.h"

static const float RAD_TO_DEG = 57.295779513f;

class LeapListener : public Leap::Listener
{
	private:
		LeapData* leapDataPtr;
		GLfloat lastLeapUpdate = 0, timeDiff = 0;
		const int LEAP_UPDATES_PER_SEC = 60;
		int handMode = HANDMODE_SLICE;
		bool startMovement;
		Leap::Vector posHandRight, posHandLeft;

	public:
		LeapListener();
		~LeapListener();

		static enum HandMode { HANDMODE_SLICE, HANDMODE_FIST, HANDMODE_FINGER, HANDMODE_ZOOM };
		virtual void onInit(const Leap::Controller &);
		virtual void onFrame(const Leap::Controller &);
		void LeapListener::setLeapData(LeapData*);
		void LeapListener::setTimeDiff(GLfloat);
		int LeapListener::getHandMode();
};