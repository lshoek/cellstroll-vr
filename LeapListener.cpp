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

	const Leap::Frame frame = controller.frame();
	if (frame.hands().isEmpty())
		return;

	//PALM TRACKING
	const Leap::Vector palmPosition = frame.hands()[0].palmPosition();
	if (palmPosition == Leap::Vector::zero())
		return;

	const Leap::Vector palmNormal = frame.hands()[0].palmNormal();
	if (palmPosition == Leap::Vector::zero())
		return;

	const bool isRight = frame.hands()[0].isRight();

	const Leap::Vector direction = frame.hands()[0].direction();
	leapDataPtr->pitch = (direction.pitch() * 180 / 3.1415926)/20;
	leapDataPtr->roll = (palmNormal.roll() * 180 / 3.1415926) / 20;
	leapDataPtr->yaw = (direction.yaw() * 180 / 3.1415926)/20;

	const Leap::FingerList fingers = frame.hands()[0].fingers();
	if (fingers.extended().count() > 4)
		handMode = HANDMODE_SLICE;
	else if (fingers.extended().count() <= 2 && !fingers.extended().fingerType(Leap::Finger::Type::TYPE_INDEX).isEmpty())
		handMode = HANDMODE_FINGER;
	else if (fingers.extended().count() == 0)
		handMode = HANDMODE_FIST;

	leapDataPtr->palmPosition = glm::vec3(palmPosition.x, palmPosition.y, palmPosition.z);
	leapDataPtr->palmNormal = glm::vec3(palmNormal.x, palmNormal.y, palmNormal.z);
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