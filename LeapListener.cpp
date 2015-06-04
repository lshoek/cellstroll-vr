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
	lastLeapUpdate += timeDiff;
	if (lastLeapUpdate <= 1.0f / LEAP_UPDATES_PER_SEC)
		return;
	lastLeapUpdate = 0;

	//PALM TRACKING
	const Leap::Frame frame = controller.frame();
	const Leap::FingerList fingers = frame.hands()[0].fingers();

	//printf("The amount of hands visible is: %d\n", frame.hands().count());

	// Rotation tracking
	const Leap::Vector normal = frame.hands()[0].palmNormal();
	const Leap::Vector direction = frame.hands()[0].direction();

	leapDataPtr->pitch = direction.pitch() * RAD_TO_DEG;
	leapDataPtr->roll = normal.roll() * RAD_TO_DEG;
	leapDataPtr->yaw = direction.yaw() * RAD_TO_DEG;

	if (frame.hands().isEmpty())
		return;

	const Leap::Vector palmPosition = frame.hands()[0].palmPosition();
	if (palmPosition == Leap::Vector::zero())
		return;
	
	if (!frame.hands().isEmpty())
	{
		//printf("The amount of fingers is: %d\n", fingers.count());
		if (fingers.extended().count() > 3)
			handMode = 1; // De hand is een slice
		else
		{
			handMode = 0; // De hand is een vuist
		}
	}

	/*
	//FINGER TRACKING
	const Leap::PointableList pointables = frame.hands()[0].pointables();
	if (pointables.isEmpty())
		return;

	Leap::Vector tipPosition = pointables[0].stabilizedTipPosition();
	if (tipPosition == Leap::Vector::zero())
		return;
	*/
	leapDataPtr->palmPosition = glm::vec3(palmPosition.x, palmPosition.y, palmPosition.z);
}

void LeapListener::setLeapData(LeapData* ptr)
{
	leapDataPtr = ptr;
}

void LeapListener::setTimeDiff(GLfloat t)
{
	timeDiff = t;
}

int LeapListener::getHandMode()
{
	return handMode;
}