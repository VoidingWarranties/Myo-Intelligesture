/* OrientationPoses extends the pose set by adding poses that depend on the
 * orientation of the arm. For example, waveIn and waveOut become waveDown and
 * waveUp if the user's arm is rotated such that they are waving up and down
 * instead of side to side.
 */

#pragma once

#include <myo/myo.hpp>
#include <string>

#include "../core/DeviceListenerWrapper.h"
#include "../core/Pose.h"
#include "Orientation.h"

namespace features {
class OrientationPoses : public core::DeviceListenerWrapper {
 public:
  class Pose : public core::Pose {
   public:
    enum Type { waveUp, waveDown, unknown };

    Pose(Type type);
    Pose(core::Pose::Type type);
    Pose(const core::Pose& pose);

    virtual std::string toString() const override;

   private:
    Type type_;
  };

  OrientationPoses(core::DeviceListenerWrapper& parent_feature,
                   Orientation& orientation);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const std::shared_ptr<core::Pose>& pose) override;

 private:
  Orientation& orientation_;
};

OrientationPoses::Pose::Pose(Type type) : core::Pose(), type_(type) {}

OrientationPoses::Pose::Pose(core::Pose::Type type)
    : core::Pose(type), type_(unknown) {}

OrientationPoses::Pose::Pose(const core::Pose& pose)
    : core::Pose(pose), type_(unknown) {}

std::string OrientationPoses::Pose::toString() const {
  switch (type_) {
    case waveUp:
      return "waveUp";
    case waveDown:
      return "waveDown";
    default:
      return core::Pose::toString();
  }
}

OrientationPoses::OrientationPoses(core::DeviceListenerWrapper& parent_feature,
                                   Orientation& orientation)
    : orientation_(orientation) {
  parent_feature.addChildFeature(this);
}

void OrientationPoses::onPose(myo::Myo* myo, uint64_t timestamp,
                              const std::shared_ptr<core::Pose>& pose) {
  Orientation::Wrist wrist_orientation = orientation_.getWristOrientation();
  if (*pose == core::Pose::waveIn) {
    std::shared_ptr<core::Pose> new_pose;
    switch (wrist_orientation) {
      case Orientation::Wrist::palmDown:
        new_pose.reset(new Pose(Pose::waveDown));
        break;
      case Orientation::Wrist::palmUp:
        new_pose.reset(new Pose(Pose::waveUp));
        break;
      default:
        core::DeviceListenerWrapper::onPose(myo, 0, pose);
        // Return so that no other poses are emitted.
        return;
    }
    core::DeviceListenerWrapper::onPose(myo, 0, new_pose);
  } else if (*pose == core::Pose::waveOut) {
    std::shared_ptr<core::Pose> new_pose;
    switch (wrist_orientation) {
      case Orientation::Wrist::palmDown:
        new_pose.reset(new Pose(Pose::waveUp));
        break;
      case Orientation::Wrist::palmUp:
        new_pose.reset(new Pose(Pose::waveDown));
        break;
      default:
        core::DeviceListenerWrapper::onPose(myo, 0, pose);
        // Return so that no other poses are emitted.
        return;
    }
    core::DeviceListenerWrapper::onPose(myo, 0, new_pose);
  } else {
    core::DeviceListenerWrapper::onPose(myo, 0, pose);
  }
}
}
