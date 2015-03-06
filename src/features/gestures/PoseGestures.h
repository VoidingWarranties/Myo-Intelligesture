/* Pose adds gesture detection for poses. Gestures include clicking,
 * double clicking, and holding the pose.
 */

#pragma once

#include <myo/myo.hpp>
#include <string>
#include <unordered_map>

#include "../../core/DeviceListenerWrapper.h"
#include "../../core/Gesture.h"
#include "../../../lib/Basic-Timer/BasicTimer.h"

namespace features {
namespace gestures {
class PoseGestures : public core::DeviceListenerWrapper {
 public:
  class Gesture : public core::Gesture {
   public:
    enum Type { singleClick, doubleClick, hold, none };

    Gesture(Type type = none);
    Gesture(const std::shared_ptr<core::Pose>& pose, Type type);

    virtual std::string toString() const override;

   private:
    Type type_;
  };

  PoseGestures(core::DeviceListenerWrapper& parent_feature,
               int click_max_hold_min = 1000, int double_click_timeout = 750);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const std::shared_ptr<core::Pose>& pose) override;
  virtual void onPeriodic(myo::Myo* myo) override;

 private:
  int click_max_hold_min_, double_click_timeout_;
  std::unordered_map<std::string, BasicTimer> gesture_timers_;
  std::shared_ptr<Gesture> last_gesture_;
};

PoseGestures::Gesture::Gesture(Type type) : core::Gesture(), type_(type) {}

PoseGestures::Gesture::Gesture(const std::shared_ptr<core::Pose>& pose,
                               Type type)
    : core::Gesture(pose), type_(type) {}

std::string PoseGestures::Gesture::toString() const {
  switch (type_) {
    case singleClick:
      return "singleClick";
    case doubleClick:
      return "doubleClick";
    case hold:
      return "hold";
    case none:
      return "none";
    default:
      return core::Gesture::toString();
  }
}

PoseGestures::PoseGestures(core::DeviceListenerWrapper& parent_feature,
                           int click_max_hold_min, int double_click_timeout)
    : click_max_hold_min_(click_max_hold_min),
      double_click_timeout_(double_click_timeout),
      last_gesture_(new Gesture()) {
  parent_feature.addChildFeature(this);
}

void PoseGestures::onPose(myo::Myo* myo, uint64_t timestamp,
                          const std::shared_ptr<core::Pose>& pose) {
  BasicTimer now;
  now.tick();

  if (gesture_timers_.count(last_gesture_->toDescriptiveString()) > 0 &&
      gesture_timers_[last_gesture_->toDescriptiveString()]
              .millisecondsSinceTick() <= click_max_hold_min_) {
    std::shared_ptr<core::Gesture> current_gesture;
    if (gesture_timers_.count(
            Gesture(last_gesture_->AssociatedPose(), Gesture::singleClick)
                .toDescriptiveString()) > 0 &&
        millisecondsBetweenTicks(
            gesture_timers_[Gesture(last_gesture_->AssociatedPose(),
                                    Gesture::singleClick)
                                .toDescriptiveString()],
            gesture_timers_[last_gesture_->toDescriptiveString()]) <=
            double_click_timeout_) {
      // Double click. Suppress the current single click.
      current_gesture.reset(
          new Gesture(last_gesture_->AssociatedPose(), Gesture::doubleClick));
    } else {
      // Single click.
      current_gesture.reset(
          new Gesture(last_gesture_->AssociatedPose(), Gesture::singleClick));
    }
    gesture_timers_[current_gesture->toDescriptiveString()] = now;
    core::DeviceListenerWrapper::onGesture(myo, timestamp, current_gesture);
  }

  last_gesture_.reset(new Gesture(pose, Gesture::none));
  gesture_timers_[last_gesture_->toDescriptiveString()] = now;
  core::DeviceListenerWrapper::onPose(myo, timestamp, pose);
}

void PoseGestures::onPeriodic(myo::Myo* myo) {
  if (*last_gesture_ != Gesture(Gesture::hold) &&
      gesture_timers_.count(Gesture(last_gesture_->AssociatedPose(),
                                    Gesture::none).toDescriptiveString()) > 0 &&
      gesture_timers_[Gesture(last_gesture_->AssociatedPose(), Gesture::none)
                          .toDescriptiveString()].millisecondsSinceTick() >
          click_max_hold_min_) {
    last_gesture_.reset(new Gesture(last_gesture_->AssociatedPose(), Gesture::hold));
    core::DeviceListenerWrapper::onGesture(myo, 0, last_gesture_);
  }
}
}
}
