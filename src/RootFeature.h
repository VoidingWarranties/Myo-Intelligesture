#ifndef MYO_INTELLIGESTURE_ROOT_FEATURE_H_
#define MYO_INTELLIGESTURE_ROOT_FEATURE_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"

class RootFeature : public DeviceListenerWrapper<myo::Pose> {
 public:
  typedef myo::Pose Pose;
};

#endif
