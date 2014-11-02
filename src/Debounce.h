#include <ctime>

#include <myo/myo.hpp>

class Debounce {
 public:
  Debounce(int debounce_delay,
           const std::function<void(myo::Myo *, uint64_t, myo::Pose)> &
               debouncedPoseCallback,
           const std::function<void(myo::Myo *)> &onPeriodicCallback)
      : debounce_delay_(debounce_delay),
        debouncedPoseCallback_(debouncedPoseCallback),
        onPeriodicCallback_(onPeriodicCallback),
        last_pose_(myo::Pose::rest),
        previous_debounced_pose_(myo::Pose::rest),
        last_pose_time_(0) {}

  void onPose(myo::Myo *myo, uint64_t timestamp, myo::Pose pose) {
    last_pose_ = pose;
    last_pose_time_ = std::clock();
  }

  void onPeriodic(myo::Myo *myo) {
    std::clock_t current_time = std::clock();
    // You would think the clock time / CLOCKS_PER_SEC would give you seconds
    // but I found you need to multiply it by 100 to get seconds. Therefore you
    // need to multiply it by 100000 to get milliseconds. Why is this so?
    int passed_milliseconds =
        float(current_time - last_pose_time_) / CLOCKS_PER_SEC * 100000;
    if (passed_milliseconds > debounce_delay_ &&
        last_pose_ != previous_debounced_pose_) {
      debouncedPoseCallback_(myo, 0, last_pose_);
      previous_debounced_pose_ = last_pose_;
      last_pose_time_ = current_time;
    }

    onPeriodicCallback_(myo);
  }

  static const int SUGGESTED_DEBOUNCE_DELAY = 25;

 private:
  int debounce_delay_;
  std::function<void(myo::Myo *, uint64_t, myo::Pose)> debouncedPoseCallback_;
  std::function<void(myo::Myo *)> onPeriodicCallback_;
  myo::Pose last_pose_, previous_debounced_pose_;
  std::clock_t last_pose_time_;
};
