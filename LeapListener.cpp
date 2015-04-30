#include "LeapListener.h"

LeapListener::LeapListener(){}
LeapListener::~LeapListener(){}


void LeapListener::onInit(const Leap::Controller &controller)
{
	if (!controller.isConnected())
		std::cout << "Please connect your Leap Motion and run the Leap application" << std::endl;
}

void LeapListener::onFrame(const Leap::Controller &controller)
{
	const Leap::Frame frame = controller.frame();
	if (frame.hands().isEmpty())
		return;

	const Leap::PointableList pointables = frame.hands()[0].pointables();
	if (pointables.isEmpty())
		return;

	Leap::Vector tipPosition = pointables[0].stabilizedTipPosition();
	if (tipPosition == Leap::Vector::zero())
		return;

	std::cout << "x:" << tipPosition.x << " y:" << tipPosition.y << " z:" << tipPosition.z << std::endl;
}