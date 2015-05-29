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

	printf("The amount of hands visible is: %d\n", frame.hands().count());

	if (frame.hands().isEmpty())
		return;

	const Leap::Vector palmPosition = frame.hands()[0].palmPosition();
	if (palmPosition == Leap::Vector::zero())
		return;
	
	if (!frame.hands().isEmpty())
	{
		printf("The amount of fingers is: %d\n", fingers.count());
		if (fingers.count() == 0)
		{
			handMode = 0;
			// De hand is een vuist
		}
		else if (fingers.count() > 3)
		{
			handMode = 1;
			// De hand is een slice
		}
		else if (frame.hands()[0].grabStrength() > 1)
		{
			handMode = 0;
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