/* PoseGestures adds gesture detection for poses. Gestures include clicking,
 * double clicking, and holding the pose.
 */

#ifndef MYO_INTELLIGESTURE_POSEGESTURES_H_
#define MYO_INTELLIGESTURE_POSEGESTURES_H_

#include <string>
#include <myo/myo.hpp>
#include "../../Basic-Timer/BasicTimer.h"

template <class BaseClass = Debounce<>, class PoseClass = Debounce<>::Pose>
class PoseGestures : public BaseClass {
 public:
  class Pose : public PoseClass {
   public:
    class Gesture {
     public:
      enum Type { none, singleClick, hold };

      Gesture(Type t = none) : gesture_(t) {}

      std::string toString() const {
        switch (gesture_) {
          case none:
            return "none";
          case singleClick:
            return "singleClick";
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
      : click_max_hold_min_(click_max_hold_min) {
    current_pose_time_.tick();
  }

  virtual void onPose(myo::Myo* myo, PoseClass pose) {
    if (current_pose_time_.millisecondsSinceTick() <= click_max_hold_min_) {
      current_pose_ = Pose(current_pose_.pose(), Pose::Gesture::singleClick);
      onPose(myo, current_pose_);
    }
    current_pose_ = Pose(pose, Pose::Gesture::none);
    current_pose_time_.tick();
  }

  virtual void onPose(myo::Myo* myo, Pose pose) {
    std::cout << pose << std::endl;
  }

  virtual void onPeriodic(myo::Myo* myo) {
    BaseClass::onPeriodic(myo);

    if (current_pose_.gesture() != Pose::Gesture::hold &&
        current_pose_time_.millisecondsSinceTick() > click_max_hold_min_) {
      current_pose_ = Pose(current_pose_.pose(), Pose::Gesture::hold);
      onPose(myo, current_pose_);
    }
  }

 private:
  int click_max_hold_min_;
  Pose current_pose_;
  BasicTimer current_pose_time_;
};

#endif
