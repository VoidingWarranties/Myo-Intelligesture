#include <myo/myo.hpp>

class AtomicEventListener : public myo::DeviceListener {
 public:
  AtomicEventListener(
      std::function<void(myo::Myo *, uint64_t, myo::Pose)> onPoseCallback)
      : onPoseCallback_(onPoseCallback) {}

  void onPose(myo::Myo *myo, uint64_t timestamp, myo::Pose pose) {
    onPoseCallback_(myo, timestamp, pose);
  }

 private:
  std::function<void(myo::Myo *, uint64_t, myo::Pose)> onPoseCallback_;
};
