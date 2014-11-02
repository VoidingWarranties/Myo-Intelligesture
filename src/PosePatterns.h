#include <myo/myo.hpp>

class PosePatterns {
 public:
  enum Pattern { singleClick, doubleClick, hold, nothing };

  PosePatterns(
      int max_delay,
      const std::function<void(myo::Myo*, uint64_t, myo::Pose,
                               PosePatterns::Pattern)>& onPosePatternCallback,
      const std::function<void(myo::Myo*)>& onPeriodicCallback)
      : max_delay_(max_delay),
        onPosePatternCallback_(onPosePatternCallback),
        onPeriodicCallback_(onPeriodicCallback),
        last_clicked_pose_(myo::Pose::rest),
        last_clicked_pose_gesture_(PoseGestures::Gesture::nothing),
        last_clicked_pose_time_(0) {}

  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose,
              PoseGestures::Gesture gesture) {
    if (gesture == PoseGestures::singleClick) {
      std::clock_t current_time = std::clock();
      int passed_milliseconds = float(current_time - last_clicked_pose_time_) /
                                CLOCKS_PER_SEC * 100000;
      if (last_clicked_pose_gesture_ == PoseGestures::singleClick &&
          passed_milliseconds <= max_delay_) {
        onPosePatternCallback_(myo, 0, pose, doubleClick);
      } else {
        onPosePatternCallback_(myo, 0, pose, singleClick);
      }
      last_clicked_pose_ = pose;
      last_clicked_pose_gesture_ = gesture;
      last_clicked_pose_time_ = current_time;
    } else {
      switch (gesture) {
        case PoseGestures::singleClick:
          onPosePatternCallback_(myo, 0, pose, singleClick);
          break;
        case PoseGestures::hold:
          onPosePatternCallback_(myo, 0, pose, hold);
          break;
        case PoseGestures::nothing:
          onPosePatternCallback_(myo, 0, pose, nothing);
          break;
      }
    }
  }

  void onPeriodic(myo::Myo* myo) { onPeriodicCallback_(myo); }

  static const int SUGGESTED_MAX_DELAY = 750;

 private:
  int max_delay_;
  std::function<void(myo::Myo*, uint64_t, myo::Pose, PosePatterns::Pattern)>
      onPosePatternCallback_;
  std::function<void(myo::Myo*)> onPeriodicCallback_;
  myo::Pose last_clicked_pose_;
  PoseGestures::Gesture last_clicked_pose_gesture_;
  std::clock_t last_clicked_pose_time_;
};
