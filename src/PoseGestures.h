/* PoseGestures adds gesture detection for poses. Gestures include clicking,
 * double clicking, and holding the pose.
 */

#ifndef MYO_INTELLIGESTURE_POSEGESTURES_H_
#define MYO_INTELLIGESTURE_POSEGESTURES_H_

#include <string>
#include <boost/circular_buffer.hpp>
#include <myo/myo.hpp>
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
        pose_buffer_(3),
        pose_time_buffer_(3) {
    for (size_t i = 0; i < 3; ++i) {
      pose_buffer_.push_front(Pose(PoseClass(Pose::rest), Pose::Gesture::hold));
      pose_time_buffer_.push_front(BasicTimer());
      pose_time_buffer_.front().tick();
    }
  }

  virtual void onPose(myo::Myo* myo, PoseClass pose) {
    if (pose_time_buffer_.front().millisecondsSinceTick() <=
        click_max_hold_min_) {
      if (pose_buffer_.front().pose() == pose_buffer_.back().pose() &&
          millisecondsBetweenTicks(pose_time_buffer_[2],
                                   pose_time_buffer_[1]) <=
              click_max_hold_min_ &&
          millisecondsBetweenTicks(pose_time_buffer_[1],
                                   pose_time_buffer_[0]) <=
              click_max_hold_min_) {
        pose_buffer_.front() =
            Pose(pose_buffer_.front().pose(), Pose::Gesture::doubleClick);
        onPose(myo, pose_buffer_.front());
      } else {
        pose_buffer_.front() =
            Pose(pose_buffer_.front().pose(), Pose::Gesture::singleClick);
        onPose(myo, pose_buffer_.front());
      }
    } else {
      pose_buffer_.front() = Pose(pose, Pose::Gesture::none);
      onPose(myo, pose_buffer_.front());
    }
    pose_time_buffer_.push_front(BasicTimer());
    pose_time_buffer_.front().tick();
    pose_buffer_.push_front(Pose(pose, Pose::Gesture::none));
  }

  virtual void onPose(myo::Myo* myo, Pose pose) {}

  virtual void onPeriodic(myo::Myo* myo) {
    BaseClass::onPeriodic(myo);

    if (pose_buffer_.front().gesture() != Pose::Gesture::hold &&
        pose_time_buffer_.front().millisecondsSinceTick() >
            click_max_hold_min_) {
      pose_buffer_.front() =
          Pose(pose_buffer_.front().pose(), Pose::Gesture::hold);
      onPose(myo, pose_buffer_.front());
    }
  }

 private:
  int click_max_hold_min_;
  boost::circular_buffer<Pose> pose_buffer_;
  boost::circular_buffer<BasicTimer> pose_time_buffer_;
};

#endif
