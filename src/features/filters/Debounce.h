/* Debounce class debounces poses to reduce accidental poses. Only poses held
 * for at least the debounce delay will trigger a pose. A pose which is held for
 * longer than the debounce delay will trigger the virtual function
 * onPose(myo::Myo*, Pose).
 */

#pragma once

#include <myo/myo.hpp>
#include <map>

#include "../../core/DeviceListenerWrapper.h"
#include "../../core/Pose.h"
#include "../../../lib/Basic-Timer/BasicTimer.h"

namespace features {
namespace filters {
class Debounce : public core::DeviceListenerWrapper {
 public:
  Debounce(core::DeviceListenerWrapper& parent_feature, int timeout_ms = 10);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const std::shared_ptr<core::Pose>& pose) override;
  virtual void onPeriodic(myo::Myo* myo) override;

 private:
  void debounceLastPose(myo::Myo* myo);

  const int timeout_ms_;
  std::map<myo::Myo*, std::shared_ptr<core::Pose>> last_pose_, last_debounced_pose_;
  std::map<myo::Myo*, BasicTimer> last_pose_time_;
  std::map<myo::Myo*, uint64_t> last_pose_timestamp_;
};

Debounce::Debounce(core::DeviceListenerWrapper& parent_feature, int timeout_ms)
    : timeout_ms_(timeout_ms) {
  parent_feature.addChildFeature(this);
}

void Debounce::onPose(myo::Myo* myo, uint64_t timestamp,
                      const std::shared_ptr<core::Pose>& pose) {
  if (last_pose_timestamp_.count(myo) > 0 &&
      timestamp - last_pose_timestamp_[myo] > 1000 * timeout_ms_ &&
      last_pose_.count(myo) > 0 &&
      (last_debounced_pose_.count(myo) == 0 ||
       *(last_pose_[myo]) != *(last_debounced_pose_[myo]))) {
    debounceLastPose(myo);
  }
  last_pose_[myo] = pose;
  last_pose_time_[myo].tick();
  last_pose_timestamp_[myo] = timestamp;
  // Don't debounce doubleTaps because of their uniqely short duration.
  if (*pose == core::Pose::doubleTap) {
    last_debounced_pose_[myo] = pose;
    core::DeviceListenerWrapper::onPose(myo, 0, pose);
  }
}

void Debounce::onPeriodic(myo::Myo* myo) {
  if (last_pose_time_.count(myo) > 0 &&
      last_pose_time_[myo].millisecondsSinceTick() > timeout_ms_ &&
      last_pose_.count(myo) > 0 &&
      (last_debounced_pose_.count(myo) == 0 ||
       *(last_pose_[myo]) != *(last_debounced_pose_[myo]))) {
    debounceLastPose(myo);
  }
  core::DeviceListenerWrapper::onPeriodic(myo);
}

void Debounce::debounceLastPose(myo::Myo* myo) {
  last_debounced_pose_[myo] = last_pose_[myo];
  last_pose_time_[myo].tick();
  core::DeviceListenerWrapper::onPose(myo, last_pose_timestamp_[myo],
                                      last_pose_[myo]);
}
}
}
