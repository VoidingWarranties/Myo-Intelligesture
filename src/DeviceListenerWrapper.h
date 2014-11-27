/* DeviceListenerWrapper wraps the myo::DeviceListener class in order to add the
 * virtual function onPeriodic(myo::Myo*) in order to allow the derived classes
 * to call Base::onPeriodic.
 */

#ifndef MYO_INTELLIGESTURE_DEVICE_LISTENER_WRAPPER_H_
#define MYO_INTELLIGESTURE_DEVICE_LISTENER_WRAPPER_H_

#include <set>
#include <algorithm>
#include <myo/myo.hpp>

template <class PoseClass>
class DeviceListenerWrapper : public myo::DeviceListener {
 protected:
  typedef DeviceListenerWrapper<PoseClass>* child_feature_t;
  std::set<child_feature_t> child_features_;

 public:
  void addChildFeature(child_feature_t feature) {
    child_features_.insert(feature);
  }
  void removeChildFeature(child_feature_t feature) {
    child_features_.erase(feature);
  }

  virtual void onPose(myo::Myo* myo, uint64_t timestamp, PoseClass pose) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [myo, timestamp, pose](child_feature_t feature) {
      feature->onPose(myo, timestamp, pose);
    });
  }
  virtual void onPeriodic(myo::Myo* myo) {
    std::for_each(child_features_.begin(), child_features_.end(),
                  [myo](child_feature_t feature) {
      feature->onPeriodic(myo);
    });
  }
};

#endif
