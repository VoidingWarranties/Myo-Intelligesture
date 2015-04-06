/* Uses the Myo's orientation to rotate accelerometer and / or gyroscope data
 * such that it is the same regardless of orientation. This makes accelerometer
 * and gyroscope data consistent no matter the Myo's orientation on the arm.
 */

#pragma once

#include <myo/myo.hpp>

#include "../core/DeviceListenerWrapper.h"

namespace features {
class CorrectForOrientation : public core::DeviceListenerWrapper {
 public:
  enum DataFlags {
    AccelerometerData = 1 << 0,
    GyroscopeData     = 1 << 1
  };

  explicit CorrectForOrientation(core::DeviceListenerWrapper& parent_feature,
                                 DataFlags flags);

  virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                 const myo::Quaternion<float>& quat) override;
  virtual void onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                   const myo::Vector3<float>& accel) override;
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) override;

 private:
  const DataFlags flags_;
  myo::Quaternion<float> last_quat_;
};

CorrectForOrientation::DataFlags operator|(
    CorrectForOrientation::DataFlags lhs,
    CorrectForOrientation::DataFlags rhs) {
  return static_cast<CorrectForOrientation::DataFlags>(static_cast<int>(lhs) |
                                                       static_cast<int>(rhs));
}

CorrectForOrientation::CorrectForOrientation(
    core::DeviceListenerWrapper& parent_feature, DataFlags flags)
    : flags_(flags), last_quat_(0, 0, 0, 1) {
  parent_feature.addChildFeature(this);
}

void CorrectForOrientation::onOrientationData(
    myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& quat) {
  last_quat_ = quat;
  core::DeviceListenerWrapper::onOrientationData(myo, timestamp, quat);
}

void CorrectForOrientation::onAccelerometerData(
    myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& accel) {
  if (flags_ & AccelerometerData) {
    auto rotated = rotate(last_quat_, accel);
    core::DeviceListenerWrapper::onAccelerometerData(myo, timestamp, rotated);
  } else {
    core::DeviceListenerWrapper::onAccelerometerData(myo, timestamp, accel);
  }
}

void CorrectForOrientation::onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                                            const myo::Vector3<float>& gyro) {
  if (flags_ & GyroscopeData) {
    auto rotated = rotate(last_quat_, gyro);
    core::DeviceListenerWrapper::onGyroscopeData(myo, timestamp, rotated);
  } else {
    core::DeviceListenerWrapper::onGyroscopeData(myo, timestamp, gyro);
  }
}
}
