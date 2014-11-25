#ifndef MYO_INTELLIGESTURE_ROOT_FEATURE_H_
#define MYO_INTELLIGESTURE_ROOT_FEATURE_H_

#include <set>
#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"

class RootFeature : public myo::DeviceListener {
 public:
  typedef myo::Pose Pose;
  void addListener(DeviceListenerWrapper<myo::Pose>* listener) {
    listeners_.insert(listener);
  }
  void removeListener(DeviceListenerWrapper<myo::Pose>* listener) {
    listeners_.erase(listener);
  }
  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    std::for_each(listeners_.begin(), listeners_.end(),
                  [myo, timestamp, pose](listener_t listener) {
      listener->onPose(myo, timestamp, pose);
    });
  }
  void onPeriodic(myo::Myo* myo) {
    std::for_each(listeners_.begin(), listeners_.end(),
                  [myo](listener_t listener) {
      listener->onPeriodic(myo);
    });
  }

 private:
  typedef DeviceListenerWrapper<myo::Pose>* listener_t;
  std::set<listener_t> listeners_;
};

#endif
