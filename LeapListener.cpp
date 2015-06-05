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

	const bool isRight = frame.hands()[0].isRight();

	if (!frame.hands().isEmpty())
	{
		if (fingers.extended().count() > 3)
			handMode = HANDMODE_SLICE;
		else if (fingers.extended().count() == 1 && !fingers.extended().fingerType(Leap::Finger::Type::TYPE_INDEX).isEmpty())
			handMode = HANDMODE_FINGER;
		else if (!fingers.extended().fingerType(Leap::Finger::Type::TYPE_INDEX).isEmpty())
			handMode = HANDMODE_FIST;
	}

	leapDataPtr->palmPosition = glm::vec3(palmPosition.x, palmPosition.y, palmPosition.z);
	leapDataPtr->palmNormal = glm::vec3(plamNormal.x, plamNormal.y, plamNormal.z);
	leapDataPtr->isRight = isRight;
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