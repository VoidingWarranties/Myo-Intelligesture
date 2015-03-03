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
template <class ParentFeature>
class OrientationPoses : public core::DeviceListenerWrapper {
  typedef typename ParentFeature::Pose ParentPose;

 public:
  class Pose : public ParentPose {
   public:
    enum Type { waveUp, waveDown, unknown };

    Pose(Type type);
    Pose(typename ParentPose::Type type);
    Pose(const ParentPose& pose);

    virtual std::string toString() const override;

   private:
    Type type_;
  };

  OrientationPoses(ParentFeature& parent_feature, Orientation& orientation);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const core::Pose& pose) override;

 private:
  Orientation& orientation_;
};

template <class ParentFeature>
OrientationPoses<ParentFeature>::Pose::Pose(Type type)
    : ParentPose(), type_(type) {}

template <class ParentFeature>
OrientationPoses<ParentFeature>::Pose::Pose(typename ParentPose::Type type)
    : ParentPose(type), type_(unknown) {}

template <class ParentFeature>
OrientationPoses<ParentFeature>::Pose::Pose(const ParentPose& pose)
    : ParentPose(pose), type_(unknown) {}

template <class ParentFeature>
std::string OrientationPoses<ParentFeature>::Pose::toString() const {
  switch (type_) {
    case waveUp:
      return "waveUp";
    case waveDown:
      return "waveDown";
    default:
      return ParentPose::toString();
  }
}

template <class ParentFeature>
OrientationPoses<ParentFeature>::OrientationPoses(ParentFeature& parent_feature,
                                                  Orientation& orientation)
    : orientation_(orientation) {
  parent_feature.addChildFeature(this);
}

template <class ParentFeature>
void OrientationPoses<ParentFeature>::onPose(myo::Myo* myo, uint64_t timestamp,
                                             const core::Pose& pose) {
  ParentPose parent_pose = static_cast<const ParentPose&>(pose);
  Orientation::Wrist wrist_orientation = orientation_.getWristOrientation();
  if (parent_pose == ParentPose::waveIn) {
    switch (wrist_orientation) {
      case Orientation::Wrist::palmDown:
        core::DeviceListenerWrapper::onPose(myo, 0, Pose(Pose::waveDown));
        break;
      case Orientation::Wrist::palmUp:
        core::DeviceListenerWrapper::onPose(myo, 0, Pose(Pose::waveUp));
        break;
      default:
        core::DeviceListenerWrapper::onPose(myo, 0, Pose(ParentPose::waveIn));
        break;
    }
  } else if (parent_pose == ParentPose::waveOut) {
    switch (wrist_orientation) {
      case Orientation::Wrist::palmDown:
        core::DeviceListenerWrapper::onPose(myo, 0, Pose(Pose::waveUp));
        break;
      case Orientation::Wrist::palmUp:
        core::DeviceListenerWrapper::onPose(myo, 0, Pose(Pose::waveDown));
        break;
      default:
        core::DeviceListenerWrapper::onPose(myo, 0, Pose(ParentPose::waveOut));
        break;
    }
  } else {
    core::DeviceListenerWrapper::onPose(myo, 0, Pose(parent_pose));
  }
}

template <class ParentFeature>
OrientationPoses<ParentFeature> make_orientation_poses(
    ParentFeature& parent_feature, Orientation& orientation) {
  return OrientationPoses<ParentFeature>(parent_feature, orientation);
}
}
