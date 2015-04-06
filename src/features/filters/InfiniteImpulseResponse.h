/* An abstract base class to derive from to create Infinite Impulse Response
 * (IIR) filters. Unlike Finite Impulse Response (FIR) filters, IIR filters only
 * store on data point. A simple example of an IIR filter is an exponential
 * moving average. An exponential moving average filter is provided in
 * ExponentialMovingAverage.h
 */

#pragma once

#include <myo/myo.hpp>
#include <map>

#include "../../core/DeviceListenerWrapper.h"

namespace features {
namespace filters {
class InfiniteImpulseResponse : public core::DeviceListenerWrapper {
 public:
  enum DataFlags {
    OrientationData   = 1 << 0,
    AccelerometerData = 1 << 1,
    GyroscopeData     = 1 << 2
  };

  explicit InfiniteImpulseResponse(core::DeviceListenerWrapper& parent_feature,
                                   DataFlags flags);

  virtual void onOrientationData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Quaternion<float>& rotation) override;
  virtual void onAccelerometerData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Vector3<float>& acceleration) override;
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) override;

 protected:
  virtual float Update(float new_value, float old_value) = 0;
  void UpdateOrientationData(myo::Myo* myo, const myo::Quaternion<float>& data);
  void UpdateAccelerometerData(myo::Myo* myo, const myo::Vector3<float>& data);
  void UpdateGyroscopeData(myo::Myo* myo, const myo::Vector3<float>& data);

  DataFlags flags_;
  std::map<myo::Myo*, myo::Quaternion<float>> orientation_data_;
  std::map<myo::Myo*, myo::Vector3<float>> accelerometer_data_;
  std::map<myo::Myo*, myo::Vector3<float>> gyroscope_data_;
};

InfiniteImpulseResponse::DataFlags operator|(
    InfiniteImpulseResponse::DataFlags lhs,
    InfiniteImpulseResponse::DataFlags rhs) {
  return static_cast<InfiniteImpulseResponse::DataFlags>(static_cast<int>(lhs) |
                                                         static_cast<int>(rhs));
}

InfiniteImpulseResponse::InfiniteImpulseResponse(
    core::DeviceListenerWrapper& parent_feature, DataFlags flags)
    : flags_(flags) {
  parent_feature.addChildFeature(this);
}

void InfiniteImpulseResponse::onOrientationData(
    myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation) {
  if (flags_ & OrientationData) {
    UpdateOrientationData(myo, rotation);
    core::DeviceListenerWrapper::onOrientationData(myo, timestamp,
                                                   orientation_data_[myo]);
  } else {
    core::DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
  }
}

void InfiniteImpulseResponse::onAccelerometerData(
    myo::Myo* myo, uint64_t timestamp,
    const myo::Vector3<float>& acceleration) {
  if (flags_ & AccelerometerData) {
    UpdateAccelerometerData(myo, acceleration);
    core::DeviceListenerWrapper::onAccelerometerData(myo, timestamp,
                                                     accelerometer_data_[myo]);
  } else {
    core::DeviceListenerWrapper::onAccelerometerData(myo, timestamp,
                                                     acceleration);
  }
}

void InfiniteImpulseResponse::onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                                              const myo::Vector3<float>& gyro) {
  if (flags_ & GyroscopeData) {
    UpdateGyroscopeData(myo, gyro);
    core::DeviceListenerWrapper::onGyroscopeData(myo, timestamp,
                                                 gyroscope_data_[myo]);
  } else {
    core::DeviceListenerWrapper::onGyroscopeData(myo, timestamp, gyro);
  }
}

void InfiniteImpulseResponse::UpdateOrientationData(
    myo::Myo* myo, const myo::Quaternion<float>& data) {
  if (orientation_data_.count(myo) == 0) {
    orientation_data_[myo] = data;
  } else {
    float x = Update(data.x(), orientation_data_[myo].x());
    float y = Update(data.y(), orientation_data_[myo].y());
    float z = Update(data.z(), orientation_data_[myo].z());
    float w = Update(data.w(), orientation_data_[myo].w());
    orientation_data_[myo] = myo::Quaternion<float>(x, y, z, w);
  }
}

void InfiniteImpulseResponse::UpdateAccelerometerData(
    myo::Myo* myo, const myo::Vector3<float>& data) {
  if (accelerometer_data_.count(myo) == 0) {
    accelerometer_data_[myo] = data;
  } else {
    float x = Update(data.x(), accelerometer_data_[myo].x());
    float y = Update(data.y(), accelerometer_data_[myo].y());
    float z = Update(data.z(), accelerometer_data_[myo].z());
    accelerometer_data_[myo] = myo::Vector3<float>(x, y, z);
  }
}

void InfiniteImpulseResponse::UpdateGyroscopeData(
    myo::Myo* myo, const myo::Vector3<float>& data) {
  if (gyroscope_data_.count(myo) == 0) {
    gyroscope_data_[myo] = data;
  } else {
    float x = Update(data.x(), gyroscope_data_[myo].x());
    float y = Update(data.y(), gyroscope_data_[myo].y());
    float z = Update(data.z(), gyroscope_data_[myo].z());
    gyroscope_data_[myo] = myo::Vector3<float>(x, y, z);
  }
}
}
}
