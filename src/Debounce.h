/* Debounce class debounces poses to reduce accidental poses. Only poses held
 * for at least the debounce delay will trigger a pose. A pose which is held for
 * longer than the debounce delay will trigger the virtual function
 * onPose(myo::Myo*, Pose).
 */

#ifndef MYO_INTELLIGESTURE_DEBOUNCE_H_
#define MYO_INTELLIGESTURE_DEBOUNCE_H_

#include <myo/myo.hpp>
#include "OrientationPoses.h"
#include "../../Basic-Timer/BasicTimer.h"

template <class BaseClass = OrientationPoses<>, class PoseClass = OrientationPoses<>::Pose>
class Debounce : public BaseClass {
 public:
  class Pose : public PoseClass {
   public:
    Pose(PoseClass pose) : PoseClass(pose) {}
    Pose(typename PoseClass::Type t) : PoseClass(t) {}
  };

  Debounce(int debounce_delay = 10)
      : debounce_delay_(debounce_delay),
        last_pose_(PoseClass::rest),
        previous_debounced_pose_(PoseClass::rest)
  {
    last_pose_time_.tick();
  }

  virtual void onPose(myo::Myo* myo, PoseClass pose) {
    last_pose_ = pose;
    last_pose_time_.tick();
  }

  virtual void onPose(myo::Myo* myo, Pose pose) {}

  virtual void onPeriodic(myo::Myo* myo) {
    BaseClass::onPeriodic(myo);

    uint64_t passed_milliseconds = last_pose_time_.millisecondsSinceTick();
    if (passed_milliseconds > debounce_delay_ &&
        last_pose_ != previous_debounced_pose_) {
      previous_debounced_pose_ = last_pose_;
      last_pose_time_.tick();
      onPose(myo, Pose(last_pose_));
    }
  }

 private:
  int debounce_delay_;
  PoseClass last_pose_, previous_debounced_pose_;
  BasicTimer last_pose_time_;
};

#endif
