/* Provides an easy interface for determining the basic orientation of the
 * user's arm and wrist.
 */

#pragma once

#include <myo/myo.hpp>

#include "../core/DeviceListenerWrapper.h"
#include "../core/OrientationUtility.h"

namespace features {
class Orientation : public core::DeviceListenerWrapper {
 public:
  enum class Arm { unknown, forearmLevel, forearmDown, forearmUp };
  enum class Wrist { unknown, palmSideways, palmDown, palmUp };

  Orientation(core::DeviceListenerWrapper& parent_feature);

  virtual void onOrientationData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Quaternion<float>& rotation) override;

  // Calibrate sets the "start" position to use as a reference in order to
  // determine the orientation of the user's arm. Currently this start position
  // is the user's arm fully extended directly in front.
  void calibrateOrientation();

  float getRelativeArmAngle() const;
  float getRelativeWristAngle() const;
  Arm getArmOrientation() const;
  Wrist getWristOrientation() const;

 private:
  myo::Quaternion<float> rotation_, mid_;
  static const float minArmAngle;
  static const float maxArmAngle;
  static const float minWristAngle;
  static const float maxWristAngle;
};

const float Orientation::minArmAngle = -1;
const float Orientation::maxArmAngle = 1;
const float Orientation::minWristAngle = -0.2;
const float Orientation::maxWristAngle = 0.3;

Orientation::Orientation(core::DeviceListenerWrapper& parent_feature)
    : rotation_(), mid_() {
  parent_feature.addChildFeature(this);
}

void Orientation::onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                    const myo::Quaternion<float>& rotation) {
  rotation_ = rotation;
  core::DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
}

void Orientation::calibrateOrientation() { mid_ = rotation_; }

float Orientation::getRelativeArmAngle() const {
  return core::OrientationUtility::RelativeOrientation(
      mid_, rotation_, core::OrientationUtility::QuaternionToPitch);
}

// TODO: add a multiplier because your forearm only rotates a fraction of the
// angle your wrist rotates.
float Orientation::getRelativeWristAngle() const {
  return core::OrientationUtility::RelativeOrientation(
      mid_, rotation_, core::OrientationUtility::QuaternionToRoll);
}

Orientation::Arm Orientation::getArmOrientation() const {
  float pitch_diff = core::OrientationUtility::RelativeOrientation(
      mid_, rotation_, core::OrientationUtility::QuaternionToPitch);
  if (pitch_diff < minArmAngle) {
    return Arm::forearmUp;
  } else if (pitch_diff > maxArmAngle) {
    return Arm::forearmDown;
  } else {
    return Arm::forearmLevel;
  }
}

Orientation::Wrist Orientation::getWristOrientation() const {
  float roll_diff = core::OrientationUtility::RelativeOrientation(
      mid_, rotation_, core::OrientationUtility::QuaternionToRoll);
  if (roll_diff < minWristAngle) {
    return Wrist::palmDown;
  } else if (roll_diff > maxWristAngle) {
    return Wrist::palmUp;
  } else {
    return Wrist::palmSideways;
  }
}
}
