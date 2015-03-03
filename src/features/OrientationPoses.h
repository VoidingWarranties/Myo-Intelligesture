/* OrientationPoses extends the pose set by adding poses that depend on the
 * orientation of the arm. For example, waveIn and waveOut become waveDown and
 * waveUp if the user's arm is rotated such that they are waving up and down
 * instead of side to side.
 */

#pragma once

#include <myo/myo.hpp>
#include <string>

#include "../core/DeviceListenerWrapper.h"
#include "Orientation.h"

namespace features {
template <class ParentFeature>
class OrientationPoses : public core::DeviceListenerWrapper {
  typedef typename ParentFeature::Pose ParentPose;

 public:
  class Pose : public ParentPose {
   public:
    enum Type { BASE_POSE, waveUp, waveDown };

    Pose();
    Pose(Type type);
    Pose(typename ParentPose::Type type);
    Pose(const ParentPose& pose);

    std::string toString() const;
    bool operator==(const Pose& other) const;
    bool operator!=(const Pose& other) const;

    // These funcions must be friends because of the oddities of ADL.
    // The template forces them to be inlined.
    friend std::ostream& operator<<(
        std::ostream& out,
        const typename OrientationPoses<ParentFeature>::Pose& pose) {
      return out << pose.toString();
    }
    friend bool operator==(const Pose& lhs, Type rhs) {
      if (lhs.pose_ == BASE_POSE && rhs == BASE_POSE) {
        return static_cast<const ParentPose&>(lhs) == rhs;
      } else {
        return lhs.pose_ == rhs;
      }
    }
    friend bool operator==(Type lhs, const Pose& rhs) {
      return Pose::operator==(rhs, lhs);
    }
    friend bool operator!=(const Pose& lhs, Type rhs) {
      if (lhs == BASE_POSE && rhs.pose_ == BASE_POSE) {
        return static_cast<const ParentPose&>(lhs) != rhs;
      } else {
        return lhs.pose_ != rhs;
      }
    }
    friend bool operator!=(Type lhs, const Pose& rhs) {
      return Pose::operator!=(rhs, lhs);
    }

   private:
    Type pose_;
  };

  OrientationPoses(ParentFeature& parent_feature, Orientation& orientation);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const myo::Pose& pose) override;

 private:
  Orientation& orientation_;
};

template <class ParentFeature>
OrientationPoses<ParentFeature>::Pose::Pose()
    : ParentPose(), pose_(BASE_POSE) {}

template <class ParentFeature>
OrientationPoses<ParentFeature>::Pose::Pose(Type type)
    : ParentPose(), pose_(type) {}

template <class ParentFeature>
OrientationPoses<ParentFeature>::Pose::Pose(typename ParentPose::Type type)
    : ParentPose(type), pose_(BASE_POSE) {}

template <class ParentFeature>
OrientationPoses<ParentFeature>::Pose::Pose(const ParentPose& pose)
    : ParentPose(pose), pose_(BASE_POSE) {}

template <class ParentFeature>
std::string OrientationPoses<ParentFeature>::Pose::toString() const {
  switch (pose_) {
    case BASE_POSE:
      return ParentPose::toString();
    case waveUp:
      return "waveUp";
    case waveDown:
      return "waveDown";
    default:
      return "unknown";
  }
}

template <class ParentFeature>
bool OrientationPoses<ParentFeature>::Pose::operator==(
    const Pose& other) const {
  if (this->pose_ == BASE_POSE && other.pose_ == BASE_POSE) {
    return ParentPose::operator==(other);
  } else {
    return this->pose_ == other.pose_;
  }
}

template <class ParentFeature>
bool OrientationPoses<ParentFeature>::Pose::operator!=(
    const Pose& other) const {
  if (this->pose_ == BASE_POSE && other.pose_ == BASE_POSE) {
    return ParentPose::operator!=(other);
  } else {
    return this->pose_ != other.pose_;
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
                                             const myo::Pose& pose) {
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
