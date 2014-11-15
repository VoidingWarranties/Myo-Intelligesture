/* OrientationPoses extends the pose set by adding poses that depend on the
 * orientation of the arm. For example, waveIn and waveOut become waveDown and
 * waveUp if the user's arm is rotated such that they are waving up and down
 * instead of side to side.
 */

#ifndef MYO_INTELLIGESTURE_ORIENTATIONPOSES_H_
#define MYO_INTELLIGESTURE_ORIENTATIONPOSES_H_

#include <iostream>
#include <myo/myo.hpp>
#include "Orientation.h"

template <class BaseClass = Orientation<>, class PoseClass = myo::Pose>
class OrientationPoses : public BaseClass {
 public:
  class Pose : public PoseClass {
   public:
    enum Type { BASE_POSE, waveUp, waveDown };

    Pose(PoseClass pose) : PoseClass(pose), pose_(BASE_POSE) {}
    Pose(typename PoseClass::Type t) : PoseClass(t), pose_(BASE_POSE) {}
    Pose(Type pose) : pose_(pose) {}

    std::string toString() const {
      switch (pose_) {
        case BASE_POSE:
          return PoseClass::toString();
        case waveUp:
          return "waveUp";
        case waveDown:
          return "waveDown";
        default:
          return "unknown";
      }
    }

    friend std::ostream& operator<<(std::ostream& out, const Pose& pose) {
      return out << pose.toString();
    }
    friend bool operator==(const Pose& pose, Pose::Type t) {
      return pose.pose_ == t;
    }
    friend bool operator==(Pose::Type t, const Pose& pose) {
      return pose.pose_ == t;
    }
    friend bool operator!=(const Pose& pose, Pose::Type t) {
      return pose.pose_ != t;
    }
    friend bool operator!=(Pose::Type t, const Pose& pose) {
      return pose.pose_ != t;
    }

   private:
    Type pose_;
  };

  OrientationPoses() : roll_(0), roll_mid_(0) {}

  virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                 const myo::Quaternion<float>& quat) {
    BaseClass::onOrientationData(myo, timestamp, quat);
  }

  virtual void onPose(myo::Myo* myo, PoseClass pose) {
    if (pose == PoseClass::waveIn || pose == PoseClass::waveOut) {
      typedef Orientation<>::Wrist Wrist;
      Wrist orient = this->getWristOrientation();
      if ((orient == Wrist::palmUp && pose == PoseClass::waveIn) ||
          (orient == Wrist::palmDown && pose == PoseClass::waveOut)) {
        onPose(myo, Pose::waveUp);
      } else if ((orient == Wrist::palmUp && pose == PoseClass::waveOut) ||
                 (orient == Wrist::palmDown && pose == PoseClass::waveIn)) {
        onPose(myo, Pose::waveDown);
      } else {
        onPose(myo, Pose(pose));
      }
    } else {
      onPose(myo, Pose(pose));
    }
  }

  virtual void onPose(myo::Myo* myo, Pose pose) = 0;

  void setRollMid() { roll_mid_ = roll_; }

 private:
  float roll_, roll_mid_;
};

#endif
