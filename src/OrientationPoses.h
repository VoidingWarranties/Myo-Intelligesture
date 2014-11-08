/* OrientationPoses extends the pose set by adding poses that depend on the
 * orientation of the arm. For example, waveIn and waveOut become waveDown and
 * waveUp if the user's arm is rotated such that they are waving up and down
 * instead of side to side.
 */

#ifndef MYO_INTELLIGESTURE_ORIENTATIONPOSES_H_
#define MYO_INTELLIGESTURE_ORIENTATIONPOSES_H_

template <class BaseClass = DeviceListenerWrapper, class PoseClass = myo::Pose>
class OrientationPoses : public BaseClass {
 public:
  class Pose : public PoseClass {
   public:
    enum Type { BASE_POSE, waveUp, waveDown };

    Pose(PoseClass pose) : PoseClass(pose), pose_(BASE_POSE) {}
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

    roll_ = atan2(2.0f * (quat.w() * quat.x() + quat.y() * quat.z()),
                  1.0f - 2.0f * (quat.x() * quat.x() + quat.y() * quat.y()));
  }

  virtual void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    BaseClass::onPose(myo, timestamp, pose);
  }

  virtual void onPose(myo::Myo* myo, PoseClass pose) {
    if (pose == PoseClass::waveIn || pose == PoseClass::waveOut) {
      float roll_diff = roll_ - roll_mid_;
      if (roll_diff > M_PI) {
        roll_diff -= (2 * M_PI);
      }
      if (roll_diff < -M_PI) {
        roll_diff += (2 * M_PI);
      }

      if (roll_diff < -0.2) {
        if (pose == PoseClass::waveIn) {
          onPose(myo, Pose::waveDown);
        } else {
          onPose(myo, Pose::waveUp);
        }
      } else {
        onPose(myo, Pose(pose));
      }
    } else {
      onPose(myo, Pose(pose));
    }
  }

  virtual void onPose(myo::Myo* myo, Pose pose) {}

  virtual void onPeriodic(myo::Myo* myo) { BaseClass::onPeriodic(myo); }

  void setRollMid() { roll_mid_ = roll_; }

 private:
  float roll_, roll_mid_;
};

#endif
