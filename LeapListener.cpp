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

	leapDataPtr->pitch = (direction.pitch() * 180 / 3.1415926)/20;
	leapDataPtr->roll = (normal.roll() * 180 / 3.1415926)/20;
	leapDataPtr->yaw = (direction.yaw() * 180 / 3.1415926)/20;

	if (frame.hands().isEmpty())
		return;

	const Leap::Vector palmPosition = frame.hands()[0].palmPosition();
	if (palmPosition == Leap::Vector::zero())
		return;

	const Leap::Vector plamNormal = frame.hands()[0].palmNormal();
	if (palmPosition == Leap::Vector::zero())
		return;

	if (!frame.hands().isEmpty())
	{
		//printf("The amount of fingers is: %d\n", fingers.count());
		if (fingers.extended().count() > 3)
			handMode = 1; // De hand is een slice
		else
			handMode = 0; // De hand is een vuist
	}

	leapDataPtr->palmPosition = glm::vec3(palmPosition.x, palmPosition.y, palmPosition.z);
	leapDataPtr->palmNormal = glm::vec3(plamNormal.x, plamNormal.y, plamNormal.z);
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