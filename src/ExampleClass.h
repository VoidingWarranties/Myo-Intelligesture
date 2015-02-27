#ifndef MYO_INTELLIGESTURE_EXAMPLECLASS_H_
#define MYO_INTELLIGESTURE_EXAMPLECLASS_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"

template <class ParentFeature>
class ExampleClass : public DeviceListenerWrapper {
  typedef typename ParentFeature::Pose ParentPose;

 public:
  ExampleClass(ParentFeature& parent_feature) {
    parent_feature.addChildFeature(this);
  }

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const myo::Pose& pose) override {
    ParentPose new_pose = static_cast<const ParentPose&>(pose);
    std::cout << new_pose << std::endl;
  }
};

template <class ParentFeature>
ExampleClass<ParentFeature> make_example(ParentFeature& parent_feature) {
  return ExampleClass<ParentFeature>(parent_feature);
}

#endif
