#include <myo/myo.hpp>

class PoseGestures {
 public:
  enum Gesture { singleClick, hold, nothing };

  PoseGestures(
      int max_click_time, int hold_time,
      const std::function<void(myo::Myo *, uint64_t, myo::Pose,
                               PoseGestures::Gesture)> &onPoseGestureCallback,
      const std::function<void(myo::Myo *)> &onPeriodicCallback)
      : max_click_time_(max_click_time),
        hold_time_(hold_time),
        onPoseGestureCallback_(onPoseGestureCallback),
        onPeriodicCallback_(onPeriodicCallback),
        last_pose_(myo::Pose::rest),
        last_held_pose_(myo::Pose::rest),
        last_pose_time_(0) {}

  void onPose(myo::Myo *myo, uint64_t timestamp, myo::Pose pose) {
    std::clock_t current_time = std::clock();
    int passed_milliseconds =
        float(current_time - last_pose_time_) / CLOCKS_PER_SEC * 100000;
    if (passed_milliseconds <= max_click_time_) {
      if (last_pose_ != myo::Pose::rest) {
        onPoseGestureCallback_(myo, 0, last_pose_, PoseGestures::singleClick);
        onPoseGestureCallback_(myo, 0, pose, PoseGestures::nothing);
      }
    } else {
      onPoseGestureCallback_(myo, 0, pose, PoseGestures::nothing);
    }
    last_pose_ = pose;
    last_pose_time_ = current_time;
  }

  void onPeriodic(myo::Myo *myo) {
    std::clock_t current_time = std::clock();
    int passed_milliseconds =
        float(current_time - last_pose_time_) / CLOCKS_PER_SEC * 100000;
    if (last_pose_ != last_held_pose_ && passed_milliseconds >= hold_time_) {
      last_held_pose_ = last_pose_;
      if (last_pose_ != myo::Pose::rest) {
        onPoseGestureCallback_(myo, 0, last_pose_, PoseGestures::hold);
      }
    }

    onPeriodicCallback_(myo);
  }

  static const int SUGGESTED_MAX_CLICK_TIME = 500;
  static const int SUGGESTED_HOLD_TIME = 500;

 private:
  int max_click_time_, hold_time_;
  std::function<void(myo::Myo *, uint64_t, myo::Pose, PoseGestures::Gesture)>
      onPoseGestureCallback_;
  std::function<void(myo::Myo *)> onPeriodicCallback_;
  myo::Pose last_pose_, last_held_pose_;
  std::clock_t last_pose_time_;
};
