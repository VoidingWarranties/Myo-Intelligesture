/* Prevents events from propogating to child features. Any event specified in
 * the EventFlags argument in the constructor will be blocked.
 */

#pragma once

#include <myo/myo.hpp>

#include "../core/DeviceListenerWrapper.h"

namespace features {
class Blocker : public core::DeviceListenerWrapper {
 public:
  enum EventFlags {
    Pair              = 1 << 0,
    Unpair            = 1 << 1,
    Connect           = 1 << 2,
    Disconnect        = 1 << 3,
    ArmSync           = 1 << 4,
    ArmUnsync         = 1 << 5,
    Unlock            = 1 << 6,
    Lock              = 1 << 7,
    Pose              = 1 << 8,
    Gesture           = 1 << 9,
    OrientationData   = 1 << 10,
    AccelerometerData = 1 << 11,
    GyroscopeData     = 1 << 12,
    Rssi              = 1 << 13,
    EmgData           = 1 << 14,
    Periodic          = 1 << 15
  };

  Blocker(core::DeviceListenerWrapper& parent_feature, EventFlags flags);

  virtual void onPair(myo::Myo* myo, uint64_t timestamp,
                      myo::FirmwareVersion firmware_version) override;
  virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onConnect(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmware_version) override;
  virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection x_direction) override;
  virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onUnlock(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onLock(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const std::shared_ptr<core::Pose>& pose) override;
  virtual void onGesture(
      myo::Myo* myo, uint64_t timestamp,
      const std::shared_ptr<core::Gesture>& gesture) override;
  virtual void onOrientationData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Quaternion<float>& rotation) override;
  virtual void onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                   const myo::Vector3<float>& accel) override;
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) override;
  virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) override;
  virtual void onEmgData(myo::Myo* myo, uint64_t timestamp,
                         const int8_t* emg) override;
  virtual void onPeriodic(myo::Myo* myo) override;

 private:
  const EventFlags flags_;
};

Blocker::EventFlags operator|(Blocker::EventFlags lhs,
                              Blocker::EventFlags rhs) {
  return static_cast<Blocker::EventFlags>(static_cast<int>(lhs) |
                                          static_cast<int>(rhs));
}

Blocker::Blocker(core::DeviceListenerWrapper& parent_feature, EventFlags flags)
    : flags_(flags) {
  parent_feature.addChildFeature(this);
}

void Blocker::onPair(myo::Myo* myo, uint64_t timestamp,
            myo::FirmwareVersion firmware_version) {
  if (!(flags_ & Pair)) {
    core::DeviceListenerWrapper::onPair(myo, timestamp, firmware_version);
  }
}

void Blocker::onUnpair(myo::Myo* myo, uint64_t timestamp) {
  if (!(flags_ & Unpair)) {
    core::DeviceListenerWrapper::onUnpair(myo, timestamp);
  }
}

void Blocker::onConnect(myo::Myo* myo, uint64_t timestamp,
               myo::FirmwareVersion firmware_version) {
  if (!(flags_ & Connect)) {
    core::DeviceListenerWrapper::onConnect(myo, timestamp, firmware_version);
  }
}

void Blocker::onDisconnect(myo::Myo* myo, uint64_t timestamp) {
  if (!(flags_ & Disconnect)) {
    core::DeviceListenerWrapper::onDisconnect(myo, timestamp);
  }
}

void Blocker::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
               myo::XDirection x_direction) {
  if (!(flags_ & ArmSync)) {
    core::DeviceListenerWrapper::onArmSync(myo, timestamp, arm, x_direction);
  }
}

void Blocker::onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
  if (!(flags_ & ArmUnsync)) {
    core::DeviceListenerWrapper::onArmUnsync(myo, timestamp);
  }
}

void Blocker::onUnlock(myo::Myo* myo, uint64_t timestamp) {
  if (!(flags_ & Unlock)) {
    core::DeviceListenerWrapper::onUnlock(myo, timestamp);
  }
}

void Blocker::onLock(myo::Myo* myo, uint64_t timestamp) {
  if (!(flags_ & Lock)) {
    core::DeviceListenerWrapper::onLock(myo, timestamp);
  }
}

void Blocker::onPose(myo::Myo* myo, uint64_t timestamp,
            const std::shared_ptr<core::Pose>& pose) {
  if (!(flags_ & Pose)) {
    core::DeviceListenerWrapper::onPose(myo, timestamp, pose);
  }
}

void Blocker::onGesture(myo::Myo* myo, uint64_t timestamp,
               const std::shared_ptr<core::Gesture>& gesture) {
  if (!(flags_ & Gesture)) {
    core::DeviceListenerWrapper::onGesture(myo, timestamp, gesture);
  }
}

void Blocker::onOrientationData(myo::Myo* myo, uint64_t timestamp,
                       const myo::Quaternion<float>& rotation) {
  if (!(flags_ & OrientationData)) {
    core::DeviceListenerWrapper::onOrientationData(myo, timestamp, rotation);
  }
}

void Blocker::onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                         const myo::Vector3<float>& accel) {
  if (!(flags_ & AccelerometerData)) {
    core::DeviceListenerWrapper::onAccelerometerData(myo, timestamp, accel);
  }
}

void Blocker::onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                     const myo::Vector3<float>& gyro) {
  if (!(flags_ & GyroscopeData)) {
    core::DeviceListenerWrapper::onGyroscopeData(myo, timestamp, gyro);
  }
}

void Blocker::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
  if (!(flags_ & Rssi)) {
    core::DeviceListenerWrapper::onRssi(myo, timestamp, rssi);
  }
}

void Blocker::onEmgData(myo::Myo* myo, uint64_t timestamp, const int8_t* emg) {
  if (!(flags_ & EmgData)) {
    core::DeviceListenerWrapper::onEmgData(myo, timestamp, emg);
  }
}

void Blocker::onPeriodic(myo::Myo* myo) {
  if (!(flags_ & Periodic)) {
    core::DeviceListenerWrapper::onPeriodic(myo);
  }
}
}
