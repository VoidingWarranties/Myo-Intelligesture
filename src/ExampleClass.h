#ifndef MYO_INTELLIGESTURE_EXAMPLECLASS_H_
#define MYO_INTELLIGESTURE_EXAMPLECLASS_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"
#include "Orientation.h"

template <class ParentFeature>
class ExampleClass : public DeviceListenerWrapper {
  typedef typename ParentFeature::Pose ParentPose;

 public:
  ExampleClass(ParentFeature& parent_feature, Orientation& orientation);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const myo::Pose& pose) override;

 private:
  Orientation& orientation_;
};

template <class ParentFeature>
ExampleClass<ParentFeature>::ExampleClass(ParentFeature& parent_feature,
                                          Orientation& orientation)
    : orientation_(orientation) {
  parent_feature.addChildFeature(this);
}

template <class ParentFeature>
void ExampleClass<ParentFeature>::onPose(myo::Myo* myo, uint64_t timestamp,
                                         const myo::Pose& pose) {
  ParentPose new_pose = static_cast<const ParentPose&>(pose);
  if (new_pose == ParentPose::doubleTap) {
    orientation_.calibrateOrientation();
  }
  std::cout << new_pose << std::endl;
}

template <class ParentFeature>
ExampleClass<ParentFeature> make_example(ParentFeature& parent_feature,
                                         Orientation& orientation) {
  return ExampleClass<ParentFeature>(parent_feature, orientation);
}

#endif
