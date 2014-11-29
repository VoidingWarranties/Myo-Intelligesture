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
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onPair(myo, timestamp, firmware_version);
    });
  }
  virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onUnpair(myo, timestamp);
    });
  }
  virtual void onConnect(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmware_version) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onConnect(myo, timestamp, firmware_version);
    });
  }
  virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onDisconnect(myo, timestamp);
    });
  }
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection x_direction) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onArmSync(myo, timestamp, arm, x_direction);
    });
  }
  virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onArmUnsync(myo, timestamp);
    });
  }
  virtual void onIntelligesturePose(myo::Myo* myo, uint64_t timestamp,
                                    const myo::Pose& pose) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onIntelligesturePose(myo, timestamp, pose);
    });
  }
  virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                 const myo::Quaternion<float>& rotation) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onOrientationData(myo, timestamp, rotation);
    });
  }
  virtual void onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                   const myo::Vector3<float>& acceleration) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onAccelerometerData(myo, timestamp, acceleration);
    });
  }
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onGyroscopeData(myo, timestamp, gyro);
    });
  }
  virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onRssi(myo, timestamp, rssi);
    });
  }
  virtual void onPeriodic(myo::Myo* myo) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [&](child_feature_t feature) {
      feature->onPeriodic(myo);
    });
  }
};

#endif
