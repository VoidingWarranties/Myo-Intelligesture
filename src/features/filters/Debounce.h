/* Debounce class debounces poses to reduce accidental poses. Only poses held
 * for at least the debounce delay will trigger a pose. A pose which is held for
 * longer than the debounce delay will trigger the virtual function
 * onPose(myo::Myo*, Pose).
 */

#pragma once

#include <myo/myo.hpp>

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

  int timeout_ms_;
  std::shared_ptr<core::Pose> last_pose_, last_debounced_pose_;
  BasicTimer last_pose_time_;
};

Debounce::Debounce(core::DeviceListenerWrapper& parent_feature, int timeout_ms)
    : timeout_ms_(timeout_ms),
      last_pose_(new core::Pose(core::Pose::rest)),
      last_debounced_pose_(last_pose_) {
  parent_feature.addChildFeature(this);
  last_pose_time_.tick();
}

void Debounce::onPose(myo::Myo* myo, uint64_t timestamp,
                      const std::shared_ptr<core::Pose>& pose) {
  debounceLastPose(myo);
  last_pose_ = pose;
  last_pose_time_.tick();
  // Don't debounce doubleTaps because of their uniqely short duration.
  if (*last_pose_ == core::Pose::doubleTap) {
    last_debounced_pose_ = last_pose_;
    core::DeviceListenerWrapper::onPose(myo, 0, last_pose_);
  }
}

void Debounce::onPeriodic(myo::Myo* myo) {
  debounceLastPose(myo);
  core::DeviceListenerWrapper::onPeriodic(myo);
}

void Debounce::debounceLastPose(myo::Myo* myo) {
  uint64_t passed_milliseconds = last_pose_time_.millisecondsSinceTick();
  if (passed_milliseconds > timeout_ms_ &&
      *last_pose_ != *last_debounced_pose_) {
    last_debounced_pose_ = last_pose_;
    last_pose_time_.tick();
    core::DeviceListenerWrapper::onPose(myo, 0, last_pose_);
  }
}
}
}
