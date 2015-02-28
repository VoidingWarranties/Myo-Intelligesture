/* Provides an easy interface for determining the basic orientation of the
 * user's arm and wrist.
 */

#ifndef MYO_INTELLIGESTURE_ORIENTATION_H_
#define MYO_INTELLIGESTURE_ORIENTATION_H_

#include <myo/myo.hpp>
#include "DeviceListenerWrapper.h"
#include "OrientationUtility.h"

template <class ParentFeature>
class Orientation : public DeviceListenerWrapper {
 public:
  enum class Arm { unknown, forearmLevel, forearmDown, forearmUp };
  enum class Wrist { unknown, palmSideways, palmDown, palmUp };

  Orientation(ParentFeature& parent_feature);

  virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                 const myo::Quaternion<float>& rotation) {
    rotation_ = rotation;
    DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
  }

  // Calibrate sets the "start" position to use as a reference in order to
  // determine the orientation of the user's arm. Currently this start position
  // is the user's arm fully extended directly in front.
  void calibrateOrientation() { mid_ = rotation_; }

  Arm getArmOrientation() const;
  Wrist getWristOrientation() const;

 private:
  myo::Quaternion<float> rotation_, mid_;
};

template <class ParentFeature>
Orientation<ParentFeature>::Orientation(ParentFeature& parent_feature)
    : rotation_(), mid_() {
  parent_feature.addChildFeature(this);
}

template <class ParentFeature>
typename Orientation<ParentFeature>::Arm
Orientation<ParentFeature>::getArmOrientation() const {
  float pitch_diff = OrientationUtility::RelativeOrientation(
      mid_, rotation_, OrientationUtility::QuaternionToPitch);
  if (pitch_diff < -1) {
    return Arm::forearmUp;
  } else if (pitch_diff > 1) {
    return Arm::forearmDown;
  } else {
    return Arm::forearmLevel;
  }
}

template <class ParentFeature>
typename Orientation<ParentFeature>::Wrist
Orientation<ParentFeature>::getWristOrientation() const {
  float roll_diff = OrientationUtility::RelativeOrientation(
      mid_, rotation_, OrientationUtility::QuaternionToRoll);
  if (roll_diff < -0.2) {
    return Wrist::palmDown;
  } else if (roll_diff > 0.3) {
    return Wrist::palmUp;
  } else {
    return Wrist::palmSideways;
  }
}

template <class ParentFeature>
Orientation<ParentFeature> make_orientation(ParentFeature& parent_feature) {
  return Orientation<ParentFeature>(parent_feature);
}

#endif
