#pragma once

#include <myo/myo.hpp>

#include "../core/DeviceListenerWrapper.h"

namespace features {
class RootFeature : public myo::DeviceListener,
                    public core::DeviceListenerWrapper {
 public:
  typedef myo::Pose Pose;

  virtual void onPair(myo::Myo* myo, uint64_t timestamp,
                      myo::FirmwareVersion firmware_version) override {
    core::DeviceListenerWrapper::onPair(myo, timestamp, firmware_version);
  }
  virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) override {
    core::DeviceListenerWrapper::onUnpair(myo, timestamp);
  }
  virtual void onConnect(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmware_version) override {
    core::DeviceListenerWrapper::onConnect(myo, timestamp, firmware_version);
  }
  virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) override {
    core::DeviceListenerWrapper::onDisconnect(myo, timestamp);
  }
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection x_direction) override {
    core::DeviceListenerWrapper::onArmSync(myo, timestamp, arm, x_direction);
  }
  virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) override {
    core::DeviceListenerWrapper::onArmUnsync(myo, timestamp);
  }
  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      myo::Pose pose) override {
    core::DeviceListenerWrapper::onPose(myo, timestamp, pose);
  }
  virtual void onOrientationData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Quaternion<float>& rotation) override {
    core::DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
  }
  virtual void onAccelerometerData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Vector3<float>& acceleration) override {
    core::DeviceListenerWrapper::onAccelerometerData(myo, timestamp,
                                                     acceleration);
  }
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) override {
    core::DeviceListenerWrapper::onGyroscopeData(myo, timestamp, gyro);
  }
  virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) override {
    core::DeviceListenerWrapper::onRssi(myo, timestamp, rssi);
  }
};
}
