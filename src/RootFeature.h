#ifndef MYO_INTELLIGESTURE_ROOT_FEATURE_H_
#define MYO_INTELLIGESTURE_ROOT_FEATURE_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"

class RootFeature : public myo::DeviceListener, public DeviceListenerWrapper {
 public:
  typedef myo::Pose Pose;

  virtual void onPair(myo::Myo* myo, uint64_t timestamp,
                      myo::FirmwareVersion firmware_version) override {
    DeviceListenerWrapper::onPair(myo, timestamp, firmware_version);
  }
  virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) override {
    DeviceListenerWrapper::onUnpair(myo, timestamp);
  }
  virtual void onConnect(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmware_version) override {
    DeviceListenerWrapper::onConnect(myo, timestamp, firmware_version);
  }
  virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) override {
    DeviceListenerWrapper::onDisconnect(myo, timestamp);
  }
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection x_direction) override {
    DeviceListenerWrapper::onArmSync(myo, timestamp, arm, x_direction);
  }
  virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) override {
    DeviceListenerWrapper::onArmUnsync(myo, timestamp);
  }
  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      myo::Pose pose) override {
    DeviceListenerWrapper::onPose(myo, timestamp, pose);
  }
  virtual void onOrientationData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Quaternion<float>& rotation) override {
    DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
  }
  virtual void onAccelerometerData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Vector3<float>& acceleration) override {
    DeviceListenerWrapper::onAccelerometerData(myo, timestamp, acceleration);
  }
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) override {
    DeviceListenerWrapper::onGyroscopeData(myo, timestamp, gyro);
  }
  virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) override {
    DeviceListenerWrapper::onRssi(myo, timestamp, rssi);
  }
};

#endif
