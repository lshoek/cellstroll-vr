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
	//LIMIT LEAP UPDATES
	lastLeapUpdate += timeDiff;
	if (lastLeapUpdate <= 1.0f / LEAP_UPDATES_PER_SEC)
		return;
	lastLeapUpdate = 0;

	//GET FRAME
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

	//CHECK RIGHT HAND
	const bool isRight = frame.hands()[0].isRight();

	//CHECK FINGER GESTURES
	const Leap::FingerList fingers = frame.hands()[0].fingers();
	if (fingers.extended().count() > 3)
		handMode = HANDMODE_SLICE;
	else if (fingers.extended().count() <= 3 && !fingers.extended().fingerType(Leap::Finger::Type::TYPE_INDEX).isEmpty())
		handMode = HANDMODE_FINGER;
	else if (fingers.extended().count() == 0)
		handMode = HANDMODE_FIST;

	//UPDATE LEAPDATA
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