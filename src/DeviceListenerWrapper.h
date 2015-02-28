/* DeviceListenerWrapper wraps the myo::DeviceListener class in order to add the
 * virtual function onPeriodic(myo::Myo*) in order to allow the derived classes
 * to call Base::onPeriodic.
 */

#ifndef MYO_INTELLIGESTURE_DEVICE_LISTENER_WRAPPER_H_
#define MYO_INTELLIGESTURE_DEVICE_LISTENER_WRAPPER_H_

#include <set>
#include <algorithm>
#include <myo/myo.hpp>

class DeviceListenerWrapper {
 protected:
  typedef DeviceListenerWrapper* child_feature_t;
  std::set<child_feature_t> child_features_;

 public:
  void addChildFeature(child_feature_t feature) {
    child_features_.insert(feature);
  }
  void removeChildFeature(child_feature_t feature) {
    child_features_.erase(feature);
  }

  virtual void onPair(myo::Myo* myo, uint64_t timestamp,
                      myo::FirmwareVersion firmware_version) {
    for (auto feature : child_features_) {
      feature->onPair(myo, timestamp, firmware_version);
    }
  }
  virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) {
    for (auto feature : child_features_) {
      feature->onUnpair(myo, timestamp);
    }
  }
  virtual void onConnect(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmware_version) {
    for (auto feature : child_features_) {
      feature->onConnect(myo, timestamp, firmware_version);
    }
  }
  virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) {
    for (auto feature : child_features_) {
      feature->onDisconnect(myo, timestamp);
    }
  }
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection x_direction) {
    for (auto feature : child_features_) {
      feature->onArmSync(myo, timestamp, arm, x_direction);
    }
  }
  virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
    for (auto feature : child_features_) {
      feature->onArmUnsync(myo, timestamp);
    }
  }
  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                                    const myo::Pose& pose) {
    for (auto feature : child_features_) {
      feature->onPose(myo, timestamp, pose);
    }
  }
  virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                 const myo::Quaternion<float>& rotation) {
    for (auto feature : child_features_) {
      feature->onOrientationData(myo, timestamp, rotation);
    }
  }
  virtual void onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                   const myo::Vector3<float>& acceleration) {
    for (auto feature : child_features_) {
      feature->onAccelerometerData(myo, timestamp, acceleration);
    }
  }
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) {
    for (auto feature : child_features_) {
      feature->onGyroscopeData(myo, timestamp, gyro);
    }
  }
  virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
    for (auto feature : child_features_) {
      feature->onRssi(myo, timestamp, rssi);
    }
  }
  virtual void onPeriodic(myo::Myo* myo) {
    for (auto feature : child_features_) {
      feature->onPeriodic(myo);
    }
  }
};

#endif
