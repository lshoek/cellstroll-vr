#pragma once
#include "Leap.h"

class LeapListener : public Leap::Listener
{
	public:
		LeapListener();
		~LeapListener();

		virtual void onInit(const Leap::Controller &);
		virtual void onFrame(const Leap::Controller &);
};