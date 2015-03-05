/* A basic moving average filter.
 * See FiniteImpulseResponse.h for more info on FIR filters.
 * http://en.wikipedia.org/wiki/Moving_average#Simple_moving_average
 */

#pragma once

#include <myo/myo.hpp>
#include <boost/optional.hpp>
#include <cassert>

#include "FiniteImpulseResponse.h"
#include "../../core/DeviceListenerWrapper.h"

namespace features {
namespace filters {
class MovingAverage : public FiniteImpulseResponse {
 public:
  explicit MovingAverage(core::DeviceListenerWrapper& parent_feature,
                         DataFlags flags, int window_size);

 private:
  virtual myo::Quaternion<float> RecalculateOrientation(
      const myo::Quaternion<float>& new_data,
      const boost::optional<myo::Quaternion<float>>& old_data) override;
  virtual myo::Vector3<float> RecalculateAcceleration(
      const myo::Vector3<float>& new_data,
      const boost::optional<myo::Vector3<float>>& old_data) override;
  virtual myo::Vector3<float> RecalculateGyration(
      const myo::Vector3<float>& new_data,
      const boost::optional<myo::Vector3<float>>& old_data) override;

  boost::optional<myo::Quaternion<float>> orientation_avg_;
  boost::optional<myo::Vector3<float>> accelerometer_avg_;
  boost::optional<myo::Vector3<float>> gyroscope_avg_;
};

MovingAverage::MovingAverage(core::DeviceListenerWrapper& parent_feature,
                             DataFlags flags, int window_size)
    : FiniteImpulseResponse(parent_feature, flags, window_size) {}

myo::Quaternion<float> MovingAverage::RecalculateOrientation(
    const myo::Quaternion<float>& new_data,
    const boost::optional<myo::Quaternion<float>>& old_data) {
  if (!orientation_avg_) {
    orientation_avg_ = new_data;
  } else if (!orientation_data_.full() || !old_data) {
    assert(orientation_avg_);
    float x = orientation_avg_->x() * (orientation_data_.size() - 1);
    float y = orientation_avg_->y() * (orientation_data_.size() - 1);
    float z = orientation_avg_->z() * (orientation_data_.size() - 1);
    float w = orientation_avg_->w() * (orientation_data_.size() - 1);
    x = (x + new_data.x()) / orientation_data_.size();
    y = (y + new_data.y()) / orientation_data_.size();
    z = (z + new_data.z()) / orientation_data_.size();
    w = (w + new_data.w()) / orientation_data_.size();
    orientation_avg_ = myo::Quaternion<float>(x, y, z, w);
  } else {
    assert(orientation_avg_);
    assert(old_data);
    float x = orientation_avg_->x();
    float y = orientation_avg_->y();
    float z = orientation_avg_->z();
    float w = orientation_avg_->w();
    x -= old_data->x() / orientation_data_.capacity();
    y -= old_data->y() / orientation_data_.capacity();
    z -= old_data->z() / orientation_data_.capacity();
    w -= old_data->w() / orientation_data_.capacity();
    x += new_data.x() / orientation_data_.capacity();
    y += new_data.y() / orientation_data_.capacity();
    z += new_data.z() / orientation_data_.capacity();
    w += new_data.w() / orientation_data_.capacity();
    orientation_avg_ = myo::Quaternion<float>(x, y, z, w);
  }
  return orientation_avg_.get();
}

myo::Vector3<float> MovingAverage::RecalculateAcceleration(
    const myo::Vector3<float>& new_data,
    const boost::optional<myo::Vector3<float>>& old_data) {
  if (!accelerometer_avg_) {
    accelerometer_avg_ = new_data;
  } else if (!accelerometer_data_.full() || !old_data) {
    assert(accelerometer_avg_);
    float x = accelerometer_avg_->x() * (accelerometer_data_.size() - 1);
    float y = accelerometer_avg_->y() * (accelerometer_data_.size() - 1);
    float z = accelerometer_avg_->z() * (accelerometer_data_.size() - 1);
    x = (x + new_data.x()) / accelerometer_data_.size();
    y = (y + new_data.y()) / accelerometer_data_.size();
    z = (z + new_data.z()) / accelerometer_data_.size();
    accelerometer_avg_ = myo::Vector3<float>(x, y, z);
  } else {
    assert(accelerometer_avg_);
    assert(old_data);
    float x = accelerometer_avg_->x();
    float y = accelerometer_avg_->y();
    float z = accelerometer_avg_->z();
    x -= old_data->x() / accelerometer_data_.capacity();
    y -= old_data->y() / accelerometer_data_.capacity();
    z -= old_data->z() / accelerometer_data_.capacity();
    x += new_data.x() / accelerometer_data_.capacity();
    y += new_data.y() / accelerometer_data_.capacity();
    z += new_data.z() / accelerometer_data_.capacity();
    accelerometer_avg_ = myo::Vector3<float>(x, y, z);
  }
  return accelerometer_avg_.get();
}

myo::Vector3<float> MovingAverage::RecalculateGyration(
    const myo::Vector3<float>& new_data,
    const boost::optional<myo::Vector3<float>>& old_data) {
  if (!gyroscope_avg_) {
    gyroscope_avg_ = new_data;
  } else if (!gyroscope_data_.full() || !old_data) {
    assert(gyroscope_avg_);
    float x = gyroscope_avg_->x() * (gyroscope_data_.size() - 1);
    float y = gyroscope_avg_->y() * (gyroscope_data_.size() - 1);
    float z = gyroscope_avg_->z() * (gyroscope_data_.size() - 1);
    x = (x + new_data.x()) / gyroscope_data_.size();
    y = (y + new_data.y()) / gyroscope_data_.size();
    z = (z + new_data.z()) / gyroscope_data_.size();
    gyroscope_avg_ = myo::Vector3<float>(x, y, z);
  } else {
    assert(gyroscope_avg_);
    assert(old_data);
    float x = gyroscope_avg_->x();
    float y = gyroscope_avg_->y();
    float z = gyroscope_avg_->z();
    x -= old_data->x() / gyroscope_data_.capacity();
    y -= old_data->y() / gyroscope_data_.capacity();
    z -= old_data->z() / gyroscope_data_.capacity();
    x += new_data.x() / gyroscope_data_.capacity();
    y += new_data.y() / gyroscope_data_.capacity();
    z += new_data.z() / gyroscope_data_.capacity();
    gyroscope_avg_ = myo::Vector3<float>(x, y, z);
  }
  return gyroscope_avg_.get();
}
}
}
