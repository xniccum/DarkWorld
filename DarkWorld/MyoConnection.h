#pragma once
#include "stdafx.h"
#include <myo/myo.hpp>

class MyoConnection : public myo::DeviceListener {
public:
	MyoConnection();
	void onUnpair(myo::Myo* myo, uint64_t timestamp);
	void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose);
	void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm, myo::XDirection xDirection, float rotation,
		myo::WarmupState warmupState);

	void onArmUnsync(myo::Myo* myo, uint64_t timestamp);
	bool onArm;
	bool isActive;
	myo::Arm whichArm;
	myo::Pose currentPose = myo::Pose::rest;
};