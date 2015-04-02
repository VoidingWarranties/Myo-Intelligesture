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
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection x_direction) override;

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
  Arm arm_orientation_a, arm_orientation_b;
  Wrist wrist_orientation_a, wrist_orientation_b;

  static const float minArmAngle;
  static const float maxArmAngle;
  static const float minWristAngle;
  static const float maxWristAngle;
};

const float Orientation::minArmAngle = -0.7;
const float Orientation::maxArmAngle = 0.7;
const float Orientation::minWristAngle = -0.3;
const float Orientation::maxWristAngle = 0.3;

Orientation::Orientation(core::DeviceListenerWrapper& parent_feature)
    : rotation_(),
      mid_(),
      arm_orientation_a(Arm::forearmUp),
      arm_orientation_b(Arm::forearmDown),
      wrist_orientation_a(Wrist::palmDown),
      wrist_orientation_b(Wrist::palmUp) {
  parent_feature.addChildFeature(this);
}

void Orientation::onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                    const myo::Quaternion<float>& rotation) {
  rotation_ = rotation;
  core::DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
}

void Orientation::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                            myo::XDirection x_direction) {
  if (arm == myo::armLeft) {
    std::swap(wrist_orientation_a, wrist_orientation_b);
  }
  if (x_direction == myo::xDirectionTowardElbow) {
    std::swap(arm_orientation_a, arm_orientation_b);
    std::swap(wrist_orientation_a, wrist_orientation_b);
  }
  core::DeviceListenerWrapper::onArmSync(myo, timestamp, arm, x_direction);
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
    return arm_orientation_a;
  } else if (pitch_diff > maxArmAngle) {
    return arm_orientation_b;
  } else {
    return Arm::forearmLevel;
  }
}

Orientation::Wrist Orientation::getWristOrientation() const {
  float roll_diff = core::OrientationUtility::RelativeOrientation(
      mid_, rotation_, core::OrientationUtility::QuaternionToRoll);
  if (roll_diff < minWristAngle) {
    return wrist_orientation_a;
  } else if (roll_diff > maxWristAngle) {
    return wrist_orientation_b;
  } else {
    return Wrist::palmSideways;
  }
}
}
