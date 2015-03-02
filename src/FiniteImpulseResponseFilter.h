/* An abstract base class to derive from to create Finite Impulse Response (FIR)
 * filters. A simple example for an FIR filter is a moving average with a fixed
 * window size. A moving average filter is provided in MovingAverageFilter.h
 */

#include <myo/myo.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/optional.hpp>

#include "DeviceListenerWrapper.h"

class FiniteImpulseResponseFilter : public DeviceListenerWrapper {
 public:
  enum DataFlags {
    OrientationData   = 1 << 0,
    AccelerometerData = 1 << 1,
    GyroscopeData     = 1 << 2
  };

  explicit FiniteImpulseResponseFilter(DeviceListenerWrapper& parent_feature,
                                       DataFlags flags, int window_size);

  virtual void onOrientationData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Quaternion<float>& rotation) override;
  virtual void onAccelerometerData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Vector3<float>& acceleration) override;
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) override;

 protected:
  myo::Quaternion<float> UpdateOrientationData(
      const myo::Quaternion<float>& data);
  myo::Vector3<float> UpdateAccelerationData(const myo::Vector3<float>& data);
  myo::Vector3<float> UpdateGyroscopeData(const myo::Vector3<float>& data);
  virtual myo::Quaternion<float> RecalculateOrientation(
      const myo::Quaternion<float>& new_data,
      const boost::optional<myo::Quaternion<float>>& old_data) = 0;
  virtual myo::Vector3<float> RecalculateAcceleration(
      const myo::Vector3<float>& new_data,
      const boost::optional<myo::Vector3<float>>& old_data) = 0;
  virtual myo::Vector3<float> RecalculateGyration(
      const myo::Vector3<float>& new_data,
      const boost::optional<myo::Vector3<float>>& old_data) = 0;

  DataFlags flags_;
  boost::circular_buffer<myo::Quaternion<float>> orientation_data_;
  boost::circular_buffer<myo::Vector3<float>> accelerometer_data_;
  boost::circular_buffer<myo::Vector3<float>> gyroscope_data_;
};

FiniteImpulseResponseFilter::DataFlags operator|(
    FiniteImpulseResponseFilter::DataFlags lhs,
    FiniteImpulseResponseFilter::DataFlags rhs) {
  return static_cast<FiniteImpulseResponseFilter::DataFlags>(
      static_cast<int>(lhs) | static_cast<int>(rhs));
}

FiniteImpulseResponseFilter::FiniteImpulseResponseFilter(
    DeviceListenerWrapper& parent_feature, DataFlags flags, int window_size)
    : flags_(flags),
      orientation_data_(window_size),
      accelerometer_data_(window_size),
      gyroscope_data_(window_size) {
  parent_feature.addChildFeature(this);
}

void FiniteImpulseResponseFilter::onOrientationData(
    myo::Myo* myo, uint64_t timestamp, const myo::Quaternion<float>& rotation) {
  if (flags_ & OrientationData) {
    DeviceListenerWrapper::onOrientationData(myo, timestamp,
                                             UpdateOrientationData(rotation));
  } else {
    DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
  }
}

void FiniteImpulseResponseFilter::onAccelerometerData(
    myo::Myo* myo, uint64_t timestamp,
    const myo::Vector3<float>& acceleration) {
  if (flags_ & AccelerometerData) {
    DeviceListenerWrapper::onAccelerometerData(
        myo, timestamp, UpdateAccelerationData(acceleration));
  } else {
    DeviceListenerWrapper::onAccelerometerData(myo, timestamp, acceleration);
  }
}

void FiniteImpulseResponseFilter::onGyroscopeData(
    myo::Myo* myo, uint64_t timestamp, const myo::Vector3<float>& gyro) {
  if (flags_ & GyroscopeData) {
    DeviceListenerWrapper::onGyroscopeData(myo, timestamp,
                                           UpdateGyroscopeData(gyro));
  } else {
    DeviceListenerWrapper::onGyroscopeData(myo, timestamp, gyro);
  }
}

myo::Quaternion<float> FiniteImpulseResponseFilter::UpdateOrientationData(
    const myo::Quaternion<float>& data) {
  boost::optional<myo::Quaternion<float>> old_data;
  if (orientation_data_.full()) {
    old_data = orientation_data_.front();
  }
  orientation_data_.push_back(data);
  return RecalculateOrientation(data, old_data);
}

myo::Vector3<float> FiniteImpulseResponseFilter::UpdateAccelerationData(
    const myo::Vector3<float>& data) {
  boost::optional<myo::Vector3<float>> old_data;
  if (accelerometer_data_.full()) {
    old_data = accelerometer_data_.front();
  }
  accelerometer_data_.push_back(data);
  return RecalculateAcceleration(data, old_data);
}

myo::Vector3<float> FiniteImpulseResponseFilter::UpdateGyroscopeData(
    const myo::Vector3<float>& data) {
  boost::optional<myo::Vector3<float>> old_data;
  if (gyroscope_data_.full()) {
    old_data = gyroscope_data_.front();
  }
  gyroscope_data_.push_back(data);
  return RecalculateGyration(data, old_data);
}
