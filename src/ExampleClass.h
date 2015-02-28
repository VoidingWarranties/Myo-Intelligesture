#ifndef MYO_INTELLIGESTURE_EXAMPLECLASS_H_
#define MYO_INTELLIGESTURE_EXAMPLECLASS_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"
#include "Orientation.h"

template <class ParentFeature>
class ExampleClass : public DeviceListenerWrapper {
  typedef typename ParentFeature::Pose ParentPose;

 public:
  ExampleClass(ParentFeature& parent_feature,
               Orientation<RootFeature>& orientation);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const myo::Pose& pose) override;

 private:
  Orientation<RootFeature>& orientation_;
};

template <class ParentFeature>
ExampleClass<ParentFeature>::ExampleClass(ParentFeature& parent_feature,
                                          Orientation<RootFeature>& orientation)
    : orientation_(orientation) {
  parent_feature.addChildFeature(this);
}

template <class ParentFeature>
void ExampleClass<ParentFeature>::onPose(myo::Myo* myo, uint64_t timestamp,
                                         const myo::Pose& pose) {
  ParentPose new_pose = static_cast<const ParentPose&>(pose);
  auto arm = orientation_.getArmOrientation();
  auto wrist = orientation_.getWristOrientation();
  std::string armStr = "";
  switch (arm) {
    case Orientation<RootFeature>::Arm::forearmLevel:
      armStr = "level";
      break;
    case Orientation<RootFeature>::Arm::forearmDown:
      armStr = "down";
      break;
    case Orientation<RootFeature>::Arm::forearmUp:
      armStr = "up";
      break;
    default:
      armStr = "unkown";
      break;
  }
  std::string wristStr = "";
  switch (wrist) {
    case Orientation<RootFeature>::Wrist::palmSideways:
      wristStr = "sideways";
      break;
    case Orientation<RootFeature>::Wrist::palmDown:
      wristStr = "down";
      break;
    case Orientation<RootFeature>::Wrist::palmUp:
      wristStr = "up";
      break;
    default:
      wristStr = "unkown";
      break;
  }
  std::cout << new_pose << ", wrist " << wristStr << ", arm " << armStr
            << std::endl;
}

template <class ParentFeature>
ExampleClass<ParentFeature> make_example(
    ParentFeature& parent_feature, Orientation<RootFeature>& orientation) {
  return ExampleClass<ParentFeature>(parent_feature, orientation);
}

#endif
