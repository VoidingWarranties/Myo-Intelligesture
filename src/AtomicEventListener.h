#include <myo/myo.hpp>

class AtomicEventListener : public myo::DeviceListener {
 public:
  AtomicEventListener(const std::function<void(myo::Myo *, uint64_t,
                                               myo::Pose)> &onPoseCallback,
                      const std::function<void(myo::Myo *)> &onPeriodicCallback)
      : onPoseCallback_(onPoseCallback),
        onPeriodicCallback_(onPeriodicCallback) {}

  void onPose(myo::Myo *myo, uint64_t timestamp, myo::Pose pose) {
    onPoseCallback_(myo, timestamp, pose);
  }

  void onPeriodic(myo::Myo *myo) { onPeriodicCallback_(myo); }

  void onOrientationData(myo::Myo *myo, uint64_t timestamp,
                         const myo::Quaternion<float> &quat) {
    onOrientationDataCallback_(myo, timestamp, quat);
  }

  void setOnOrientationDataCallback(const std::function<
      void(myo::Myo *, uint64_t, const myo::Quaternion<float> &quat)>& callback) {
    onOrientationDataCallback_ = callback;
  }

 private:
  std::function<void(myo::Myo *, uint64_t, myo::Pose)> onPoseCallback_;
  std::function<void(myo::Myo *)> onPeriodicCallback_;
  std::function<void(myo::Myo *, uint64_t, const myo::Quaternion<float> &)>
      onOrientationDataCallback_;
};
