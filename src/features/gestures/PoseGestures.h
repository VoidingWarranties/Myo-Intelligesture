/* Pose adds gesture detection for poses. Gestures include clicking,
 * double clicking, and holding the pose.
 */

#ifndef MYO_INTELLIGESTURE_POSEGESTURES_H_
#define MYO_INTELLIGESTURE_POSEGESTURES_H_

#include <myo/myo.hpp>
#include <string>
#include <unordered_map>

#include "../../core/DeviceListenerWrapper.h"
#include "../../../lib/Basic-Timer/BasicTimer.h"

namespace features {
namespace gestures {
template <class ParentFeature>
class PoseGestures : public core::DeviceListenerWrapper {
  typedef typename ParentFeature::Pose ParentPose;

 public:
  class Pose : public ParentPose {
   public:
    enum GestureType { none, singleClick, doubleClick, hold };

    Pose(GestureType gesture = none);
    Pose(typename ParentPose::Type type, GestureType gesture = none);
    Pose(const ParentPose& pose, GestureType gesture = none);

    std::string toString(bool gestureInfo = true) const;
    bool operator==(const Pose& other) const;
    bool operator!=(const Pose& other) const;

    friend std::ostream& operator<<(
        std::ostream& out, const typename PoseGestures<ParentFeature>::Pose& pose) {
      return out << pose.toString();
    }
    friend bool operator==(const Pose& lhs, GestureType rhs) {
      return lhs.gesture_ == rhs;
    }
    friend bool operator==(GestureType lhs, const Pose& rhs) {
      return Pose::operator==(rhs, lhs);
    }
    friend bool operator!=(const Pose& lhs, GestureType rhs) {
      return lhs.gesture_ != rhs;
    }
    friend bool operator!=(GestureType lhs, const Pose& rhs) {
      return Pose::operator!=(rhs, lhs);
    }

   private:
    GestureType gesture_;
  };

  PoseGestures(ParentFeature& parent_feature, int click_max_hold_min,
               int double_click_timeout);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const myo::Pose& pose) override;
  virtual void onPeriodic(myo::Myo* myo) override;

 private:
  int click_max_hold_min_, double_click_timeout_;
  Pose last_pose_;
  std::unordered_map<std::string, BasicTimer> last_pose_times_;
};

template <class ParentFeature>
PoseGestures<ParentFeature>::Pose::Pose(GestureType gesture)
    : ParentPose(), gesture_(gesture) {}

template <class ParentFeature>
PoseGestures<ParentFeature>::Pose::Pose(typename ParentPose::Type type,
                                        GestureType gesture)
    : ParentPose(type), gesture_(gesture) {}

template <class ParentFeature>
PoseGestures<ParentFeature>::Pose::Pose(const ParentPose& pose,
                                        GestureType gesture)
    : ParentPose(pose), gesture_(gesture) {}

template <class ParentFeature>
std::string PoseGestures<ParentFeature>::Pose::toString(
    bool gestureInfo) const {
  std::string result;
  result = ParentPose::toString();
  if (gestureInfo) {
    result += ", ";
    switch (gesture_) {
      case none:
        result += "none";
        break;
      case singleClick:
        result += "singleClick";
        break;
      case doubleClick:
        result += "doubleClick";
        break;
      case hold:
        result += "hold";
        break;
      default:
        result += "unknown";
        break;
    }
  }
  return result;
}

template <class ParentFeature>
bool PoseGestures<ParentFeature>::Pose::operator==(const Pose& other) const {
  return this->gesture_ == other.gesture_ && ParentPose::operator==(other);
}

template <class ParentFeature>
bool PoseGestures<ParentFeature>::Pose::operator!=(const Pose& other) const {
  return this->gesture_ != other.gesture_ || ParentPose::operator!=(other);
}

template <class ParentFeature>
PoseGestures<ParentFeature>::PoseGestures(ParentFeature& parent_feature,
                                          int click_max_hold_min,
                                          int double_click_timeout)
    : click_max_hold_min_(click_max_hold_min),
      double_click_timeout_(double_click_timeout) {
  parent_feature.addChildFeature(this);
}

template <class ParentFeature>
void PoseGestures<ParentFeature>::onPose(myo::Myo* myo, uint64_t timestamp,
                                         const myo::Pose& pose) {
  ParentPose parent_pose = static_cast<const ParentPose&>(pose);

  BasicTimer now;
  now.tick();

  Pose current_pose;
  Pose last_pose_gesture_none = Pose(last_pose_, Pose::none);
  Pose last_pose_gesture_singleClick = Pose(last_pose_, Pose::singleClick);

  if (last_pose_times_.count(last_pose_gesture_none.toString()) > 0 &&
      last_pose_times_[last_pose_gesture_none.toString()]
              .millisecondsSinceTick() <= click_max_hold_min_) {
    if (last_pose_times_.count(last_pose_gesture_singleClick.toString()) > 0 &&
        millisecondsBetweenTicks(
            last_pose_times_[last_pose_gesture_singleClick.toString()],
            last_pose_times_[last_pose_gesture_none.toString()]) <=
            double_click_timeout_) {
      // Double click. Suppresses the current single click.
      current_pose = Pose(last_pose_, Pose::doubleClick);
    } else {
      // Single click.
      current_pose = Pose(last_pose_, Pose::singleClick);
    }
    last_pose_times_[current_pose.toString()] = now;
    core::DeviceListenerWrapper::onPose(myo, 0, current_pose);
  } else {
    current_pose = Pose(parent_pose, Pose::none);
  }

  last_pose_ = Pose(parent_pose, Pose::none);
  last_pose_times_[last_pose_.toString()] = now;
  core::DeviceListenerWrapper::onPose(myo, 0, last_pose_);
}

template <class ParentFeature>
void PoseGestures<ParentFeature>::onPeriodic(myo::Myo* myo) {
  if (last_pose_ != Pose::hold &&
      last_pose_times_.count(Pose(last_pose_, Pose::none).toString()) > 0 &&
      last_pose_times_[Pose(last_pose_, Pose::none).toString()]
              .millisecondsSinceTick() > click_max_hold_min_) {
    last_pose_ = Pose(last_pose_, Pose::hold);
    core::DeviceListenerWrapper::onPose(myo, 0, last_pose_);
  }
}

template <class ParentFeature>
PoseGestures<ParentFeature> make_pose_gestures(ParentFeature& parent_feature,
                                               int click_max_hold_min = 1000,
                                               int double_click_timeout = 750) {
  return PoseGestures<ParentFeature>(parent_feature, click_max_hold_min,
                                     double_click_timeout);
}
}
}

#endif
