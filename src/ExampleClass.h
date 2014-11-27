#ifndef MYO_INTELLIGESTURE_EXAMPLECLASS_H_
#define MYO_INTELLIGESTURE_EXAMPLECLASS_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"

template <class ParentFeature>
class ExampleClass
    : public DeviceListenerWrapper<typename ParentFeature::Pose> {
 public:
  ExampleClass(ParentFeature& parent_feature) {
    parent_feature.addChildFeature(this);
  }

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      typename ParentFeature::Pose pose) {
    std::cout << pose << std::endl;
  }
};

template <class ParentFeature>
ExampleClass<ParentFeature> make_example(ParentFeature& parent_feature) {
  return ExampleClass<ParentFeature>(parent_feature);
}

#endif
