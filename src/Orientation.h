/* Provides an easy interface for determining the basic orientation of the
 * user's arm and wrist.
 */

#ifndef MYO_INTELLIGESTURE_ORIENTATION_H_
#define MYO_INTELLIGESTURE_ORIENTATION_H_

#include <string>
#include <iostream>
#include <myo/myo.hpp>
#include "OrientationUtility.h"

template <class BaseClass = DeviceListenerWrapper>
class Orientation : public BaseClass {
 public:
  class Arm {
   public:
    enum Type { unknown, forearmLevel, forearmDown, forearmUp };

    Arm() : arm_(unknown) {}
    Arm(const Type& t) : arm_(t) {}

    Type type() const { return arm_; }

    std::string toString() const {
      switch (arm_) {
        case forearmLevel:
          return "forearmLevel";
        case forearmDown:
          return "forearmDown";
        case forearmUp:
          return "forearmUp";
        default:
          return "unknown";
      }
    }

    friend std::ostream& operator<<(std::ostream& out, const Arm& arm) {
      return out << arm.toString();
    }
    friend bool operator==(const Arm& arm, const Type& t) {
      return arm.arm_ == t;
    }
    friend bool operator==(const Type& t, const Arm& arm) {
      return arm.arm_ == t;
    }
    friend bool operator!=(const Arm& arm, const Type& t) {
      return arm.arm_ != t;
    }
    friend bool operator!=(const Type& t, const Arm& arm) {
      return arm.arm_ != t;
    }

   private:
    Type arm_;
  };

  class Wrist {
   public:
    enum Type { unknown, palmSideways, palmDown, palmUp };

    Wrist() : wrist_(unknown) {}
    Wrist(const Type& t) : wrist_(t) {}

    Type type() const { return wrist_; }

    std::string toString() const {
      switch (wrist_) {
        case palmSideways:
          return "palmSideways";
        case palmDown:
          return "palmDown";
        case palmUp:
          return "palmUp";
        default:
          return "unknown";
      }
    }

    friend std::ostream& operator<<(std::ostream& out, const Wrist& wrist) {
      return out << wrist.toString();
    }
    friend bool operator==(const Wrist& wrist, const Type& t) {
      return wrist.wrist_ == t;
    }
    friend bool operator==(const Type& t, const Wrist& wrist) {
      return wrist.wrist_ == t;
    }
    friend bool operator!=(const Wrist& wrist, const Type& t) {
      return wrist.wrist_ != t;
    }
    friend bool operator!=(const Type& t, const Wrist& wrist) {
      return wrist.wrist_ != t;
    }

   private:
    Type wrist_;
  };

  Orientation() : rotation_(), mid_() {}

  virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                 const myo::Quaternion<float>& quat) {
    BaseClass::onOrientationData(myo, timestamp, quat);
    rotation_ = quat;
  }

  // Calibrate sets the "start" position to use as a reference in order to
  // determine the orientation of the user's arm. Currently this start position
  // is the user's arm fully extended directly in front.
  void calibrateOrientation() { mid_ = rotation_; }

  Arm getArmOrientation() const {
    float pitch_diff = OrientationUtility::RelativeOrientation(
        mid_, rotation_, OrientationUtility::QuaternionToPitch);
    if (pitch_diff < -1) {
      return Arm::forearmUp;
    } else if (pitch_diff > 1) {
      return Arm::forearmDown;
    } else {
      return Arm::forearmLevel;
    }
  }

  Wrist getWristOrientation() const {
    float roll_diff = OrientationUtility::RelativeOrientation(
        mid_, rotation_, OrientationUtility::QuaternionToRoll);
    if (roll_diff < -0.2) {
      return Wrist::palmDown;
    } else if (roll_diff > 0.3) {
      return Wrist::palmUp;
    } else {
      return Wrist::palmSideways;
    }
  }

 private:
  myo::Quaternion<float> rotation_, mid_;
};

#endif
