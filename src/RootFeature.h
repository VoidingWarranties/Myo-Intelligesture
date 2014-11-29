#ifndef MYO_INTELLIGESTURE_ROOT_FEATURE_H_
#define MYO_INTELLIGESTURE_ROOT_FEATURE_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"

class RootFeature : public myo::DeviceListener, public DeviceListenerWrapper {
 public:
  typedef myo::Pose Pose;

  virtual void onPair(myo::Myo* myo, uint64_t timestamp,
                      myo::FirmwareVersion firmware_version) {
    DeviceListenerWrapper::onPair(myo, timestamp, firmware_version);
  }
  virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) {
    DeviceListenerWrapper::onUnpair(myo, timestamp);
  }
  virtual void onConnect(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmware_version) {
    DeviceListenerWrapper::onConnect(myo, timestamp, firmware_version);
  }
  virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) {
    DeviceListenerWrapper::onDisconnect(myo, timestamp);
  }
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection x_direction) {
    DeviceListenerWrapper::onArmSync(myo, timestamp, arm, x_direction);
  }
  virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
    DeviceListenerWrapper::onArmUnsync(myo, timestamp);
  }
  virtual void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    DeviceListenerWrapper::onPose(myo, timestamp, pose);
  }
  virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                 const myo::Quaternion<float>& rotation) {
    DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
  }
  virtual void onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                   const myo::Vector3<float>& acceleration) {
    DeviceListenerWrapper::onAccelerometerData(myo, timestamp, acceleration);
  }
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) {
    DeviceListenerWrapper::onGyroscopeData(myo, timestamp, gyro);
  }
  virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
    DeviceListenerWrapper::onRssi(myo, timestamp, rssi);
  }
};

#endif
