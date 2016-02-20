#pragma once
#include "stdafx.h"
#include "MyoConnection.h"


	MyoConnection::MyoConnection()
		: onArm(false), currentPose()
	{
		isActive = false;
	}
	// onUnpair() is called whenever the Myo is disconnected from Myo Connect by the user.
	void MyoConnection::onUnpair(myo::Myo* myo, uint64_t timestamp)
	{
		onArm = false;
	}
	// onPose() is called whenever the Myo detects that the person wearing it has changed their pose, for example,
	// making a fist, or not making a fist anymore.
	void MyoConnection::onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose)
	{
		currentPose = pose;

		if (pose != myo::Pose::unknown && pose != myo::Pose::rest) {
			// Tell the Myo to stay unlocked until told otherwise. We do that here so you can hold the poses without the
			// Myo becoming locked.
			//myo->unlock(myo::Myo::unlockHold);

			// Notify the Myo that the pose has resulted in an action, in this case changing
			// the text on the screen. The Myo will vibrate.
			myo->notifyUserAction();
			
		}
		else {
			// Tell the Myo to stay unlocked only for a short period. This allows the Myo to stay unlocked while poses
			// are being performed, but lock after inactivity.
			//myo->unlock(myo::Myo::unlockTimed);
			isActive = false;
		}
	}
	// onArmSync() is called whenever Myo has recognized a Sync Gesture after someone has put it on their
	// arm. This lets Myo know which arm it's on and which way it's facing.
	void MyoConnection::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
		myo::WarmupState warmupState)
	{
		onArm = true;
		whichArm = arm;
	}

	// onArmUnsync() is called whenever Myo has detected that it was moved from a stable position on a person's arm after
	// it recognized the arm. Typically this happens when someone takes Myo off of their arm, but it can also happen
	// when Myo is moved around on the arm.
	void MyoConnection::onArmUnsync(myo::Myo* myo, uint64_t timestamp)
	{
		onArm = false;
	}


