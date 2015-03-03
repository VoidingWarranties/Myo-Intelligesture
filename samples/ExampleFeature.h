#pragma once

#include <myo/myo.hpp>

#include "../src/core/DeviceListenerWrapper.h"
#include "../src/core/Pose.h"
#include "../src/features/Orientation.h"

template <class ParentFeature>
class ExampleFeature : public core::DeviceListenerWrapper {
  typedef typename ParentFeature::Pose ParentPose;

 public:
  ExampleFeature(ParentFeature& parent_feature,
                 features::Orientation& orientation);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const core::Pose& pose) override;

 private:
  features::Orientation& orientation_;
};

template <class ParentFeature>
ExampleFeature<ParentFeature>::ExampleFeature(
    ParentFeature& parent_feature, features::Orientation& orientation)
    : orientation_(orientation) {
  parent_feature.addChildFeature(this);
}

template <class ParentFeature>
void ExampleFeature<ParentFeature>::onPose(myo::Myo* myo, uint64_t timestamp,
                                           const core::Pose& pose) {
  ParentPose new_pose = static_cast<const ParentPose&>(pose);
  if (new_pose == ParentPose::doubleTap) {
    orientation_.calibrateOrientation();
  }
  std::cout << new_pose << std::endl;
}

template <class ParentFeature>
ExampleFeature<ParentFeature> make_example(ParentFeature& parent_feature,
                                           features::Orientation& orientation) {
  return ExampleFeature<ParentFeature>(parent_feature, orientation);
}
