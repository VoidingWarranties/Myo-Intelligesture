#pragma once

#include <myo/myo.hpp>

#include "../src/core/DeviceListenerWrapper.h"
#include "../src/core/Pose.h"
#include "../src/features/Orientation.h"

class ExampleFeature : public core::DeviceListenerWrapper {
 public:
  ExampleFeature(core::DeviceListenerWrapper& parent_feature,
                 features::Orientation& orientation);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const std::shared_ptr<core::Pose>& pose) override;
  virtual void onGesture(
      myo::Myo* myo, uint64_t timestamp,
      const std::shared_ptr<core::Gesture>& gesture) override;

 private:
  features::Orientation& orientation_;
};

ExampleFeature::ExampleFeature(core::DeviceListenerWrapper& parent_feature,
                               features::Orientation& orientation)
    : orientation_(orientation) {
  parent_feature.addChildFeature(this);
}

void ExampleFeature::onPose(myo::Myo* myo, uint64_t timestamp,
                            const std::shared_ptr<core::Pose>& pose) {
  if (*pose == core::Pose::doubleTap) {
    orientation_.calibrateOrientation();
  }
}

void ExampleFeature::onGesture(myo::Myo* myo, uint64_t timestamp,
               const std::shared_ptr<core::Gesture>& gesture) {
  std::cout << gesture->toDescriptiveString() << std::endl;
}
