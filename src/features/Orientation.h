/* Provides an easy interface for determining the basic orientation of the
 * user's arm and wrist.
 */

#ifndef MYO_INTELLIGESTURE_ORIENTATION_H_
#define MYO_INTELLIGESTURE_ORIENTATION_H_

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
};

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

typename Orientation::Arm Orientation::getArmOrientation() const {
  float pitch_diff = core::OrientationUtility::RelativeOrientation(
      mid_, rotation_, core::OrientationUtility::QuaternionToPitch);
  if (pitch_diff < -1) {
    return Arm::forearmUp;
  } else if (pitch_diff > 1) {
    return Arm::forearmDown;
  } else {
    return Arm::forearmLevel;
  }
}

typename Orientation::Wrist Orientation::getWristOrientation() const {
  float roll_diff = core::OrientationUtility::RelativeOrientation(
      mid_, rotation_, core::OrientationUtility::QuaternionToRoll);
  if (roll_diff < -0.2) {
    return Wrist::palmDown;
  } else if (roll_diff > 0.3) {
    return Wrist::palmUp;
  } else {
    return Wrist::palmSideways;
  }
}

// This factory function is no longer necessary now that Orientation is not
// templated, but it is kept here for consistency.
Orientation make_orientation(core::DeviceListenerWrapper& parent_feature) {
  return Orientation(parent_feature);
}
}

#endif
