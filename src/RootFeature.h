#ifndef MYO_INTELLIGESTURE_ROOT_FEATURE_H_
#define MYO_INTELLIGESTURE_ROOT_FEATURE_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"

class RootFeature : public DeviceListenerWrapper {
 public:
  typedef myo::Pose Pose;

  virtual void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    DeviceListenerWrapper::onIntelligesturePose(myo, timestamp, pose);
  }
};

#endif
