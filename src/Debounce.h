/* Debounce class debounces poses to reduce accidental poses. Only poses held
 * for at least the debounce delay will trigger a pose. A pose which is held for
 * longer than the debounce delay will trigger the virtual function
 * onPose(myo::Myo*, Pose).
 */

#ifndef MYO_INTELLIGESTURE_DEBOUNCE_H_
#define MYO_INTELLIGESTURE_DEBOUNCE_H_

#include <ctime>
#include <myo/myo.hpp>

template <class BaseClass, class PoseClass>
class Debounce : public BaseClass {
 public:
  class Pose : public PoseClass {
   public:
    Pose(PoseClass pose) : PoseClass(pose) {}
  };

  Debounce(int debounce_delay = 10)
      : debounce_delay_(debounce_delay),
        last_pose_(PoseClass::rest),
        previous_debounced_pose_(PoseClass::rest),
        last_pose_time_(std::clock()) {}

  virtual void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    BaseClass::onPose(myo, timestamp, pose);
  }

  virtual void onPose(myo::Myo* myo, PoseClass pose) {
    last_pose_ = pose;
    last_pose_time_ = std::clock();
  }

  virtual void onPose(myo::Myo* myo, Pose pose) {
    std::cout << pose << std::endl;
    if (pose == Pose::thumbToPinky) this->setRollMid();
  }

  virtual void onPeriodic(myo::Myo* myo) {
    BaseClass::onPeriodic(myo);

    std::clock_t current_time = std::clock();
    // You would think the clock time / CLOCKS_PER_SEC would give you seconds
    // but I found you need to multiply it by 100 to get seconds. Therefore you
    // need to multiply it by 100000 to get milliseconds. Why is this so?
    int passed_milliseconds =
        float(current_time - last_pose_time_) / CLOCKS_PER_SEC * 100000;
    if (passed_milliseconds > debounce_delay_ &&
        last_pose_ != previous_debounced_pose_) {
      previous_debounced_pose_ = last_pose_;
      last_pose_time_ = current_time;
      onPose(myo, Pose(last_pose_));
    }
  }

 private:
  int debounce_delay_;
  PoseClass last_pose_, previous_debounced_pose_;
  std::clock_t last_pose_time_;
};

#endif
