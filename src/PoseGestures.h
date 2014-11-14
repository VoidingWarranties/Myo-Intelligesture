/* PoseGestures adds gesture detection for poses. Gestures include clicking,
 * double clicking, and holding the pose.
 */

#ifndef MYO_INTELLIGESTURE_POSEGESTURES_H_
#define MYO_INTELLIGESTURE_POSEGESTURES_H_

#include <string>
#include <unordered_map>
#include <myo/myo.hpp>
#include "Debounce.h"
#include "../../Basic-Timer/BasicTimer.h"

template <class BaseClass = Debounce<>, class PoseClass = Debounce<>::Pose>
class PoseGestures : public BaseClass {
 public:
  class Pose : public PoseClass {
   public:
    class Gesture {
     public:
      enum Type { none, singleClick, doubleClick, hold };

      Gesture(Type t = none) : gesture_(t) {}

      std::string toString() const {
        switch (gesture_) {
          case none:
            return "none";
          case singleClick:
            return "singleClick";
          case doubleClick:
            return "doubleClick";
          case hold:
            return "hold";
          default:
            return "unknown";
        }
      }

      friend std::ostream& operator<<(std::ostream& out, const Gesture& gest) {
        return out << gest.toString();
      }
      friend bool operator==(const Gesture& gest, Type t) {
        return gest.gesture_ == t;
      }
      friend bool operator==(Type t, const Gesture& gest) {
        return gest.gesture_ == t;
      }
      friend bool operator!=(const Gesture& gest, Type t) {
        return gest.gesture_ != t;
      }
      friend bool operator!=(Type t, const Gesture& gest) {
        return gest.gesture_ != t;
      }

     private:
      Type gesture_;
    };

    Pose(PoseClass pose = PoseClass(PoseClass::rest),
         Gesture gest = Gesture::hold)
        : PoseClass(pose), pose_(pose), gesture_(gest) {}

    PoseClass pose() const { return pose_; }
    Gesture gesture() const { return gesture_; }

    std::string toString() const {
      return pose_.toString() + ": " + gesture_.toString();
    }

    friend std::ostream& operator<<(std::ostream& out, const Pose& pose) {
      return out << pose.toString();
    }

   private:
    PoseClass pose_;
    Gesture gesture_;
  };

  PoseGestures(int click_max_hold_min = 1000)
      : click_max_hold_min_(click_max_hold_min),
        last_pose_(PoseClass(PoseClass::rest), Pose::Gesture::none) {}

  virtual void onPose(myo::Myo* myo, PoseClass pose) {
    BasicTimer now;
    now.tick();

    Pose current_pose;
    Pose last_pose_gesture_none = Pose(last_pose_.pose(), Pose::Gesture::none);
    Pose last_pose_gesture_singleClick =
        Pose(last_pose_.pose(), Pose::Gesture::singleClick);

    if (last_pose_times_.count(last_pose_gesture_none.toString()) > 0 &&
        last_pose_times_[last_pose_gesture_none.toString()]
                .millisecondsSinceTick() <= click_max_hold_min_) {
      if (last_pose_times_.count(last_pose_gesture_singleClick.toString()) >
              0 &&
          millisecondsBetweenTicks(
              last_pose_times_[last_pose_gesture_singleClick.toString()],
              last_pose_times_[last_pose_gesture_none.toString()]) <=
              click_max_hold_min_) {
        // Double click. Suppresses the current single click.
        current_pose = Pose(last_pose_.pose(), Pose::Gesture::doubleClick);
      } else {
        // Single click.
        current_pose = Pose(last_pose_.pose(), Pose::Gesture::singleClick);
      }
      last_pose_times_[current_pose.toString()] = now;
      onPose(myo, current_pose);
    } else {
      current_pose = Pose(pose, Pose::Gesture::none);
    }

    last_pose_ = Pose(pose, Pose::Gesture::none);
    last_pose_times_[last_pose_.toString()] = now;
    onPose(myo, last_pose_);
  }

  virtual void onPose(myo::Myo* myo, Pose pose) {}

  virtual void onPeriodic(myo::Myo* myo) {
    BaseClass::onPeriodic(myo);

    if (last_pose_.gesture() != Pose::Gesture::hold &&
        last_pose_times_.count(
            Pose(last_pose_.pose(), Pose::Gesture::none).toString()) > 0 &&
        last_pose_times_[Pose(last_pose_.pose(), Pose::Gesture::none)
                             .toString()].millisecondsSinceTick() >
            click_max_hold_min_) {
      last_pose_ = Pose(last_pose_.pose(), Pose::Gesture::hold);
      onPose(myo, last_pose_);
    }
  }

 private:
  int click_max_hold_min_;

  Pose last_pose_;
  std::unordered_map<std::string, BasicTimer> last_pose_times_;
};

#endif
