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
template <class ParentFeature>
class Debounce : public core::DeviceListenerWrapper {
  typedef typename ParentFeature::Pose ParentPose;

 public:
  typedef ParentPose Pose;

  Debounce(ParentFeature& parent_feature, int timeout_ms);

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const core::Pose& pose) override;
  virtual void onPeriodic(myo::Myo* myo) override;

 private:
  int timeout_ms_;
  ParentPose last_pose_, last_debounced_pose_;
  BasicTimer last_pose_time_;
};

template <class ParentFeature>
Debounce<ParentFeature>::Debounce(ParentFeature& parent_feature, int timeout_ms)
    : timeout_ms_(timeout_ms),
      last_pose_(ParentPose::rest),
      last_debounced_pose_(last_pose_) {
  parent_feature.addChildFeature(this);
  last_pose_time_.tick();
}

template <class ParentFeature>
void Debounce<ParentFeature>::onPose(myo::Myo* myo, uint64_t timestamp,
                                     const core::Pose& pose) {
  last_pose_ = static_cast<const ParentPose&>(pose);
  last_pose_time_.tick();
  // Don't debounce doubleTaps because of their uniqely short duration.
  if (last_pose_ == ParentPose::doubleTap) {
    last_debounced_pose_ = last_pose_;
    core::DeviceListenerWrapper::onPose(myo, 0, Pose(last_pose_));
  }
}

template <class ParentFeature>
void Debounce<ParentFeature>::onPeriodic(myo::Myo* myo) {
  uint64_t passed_milliseconds = last_pose_time_.millisecondsSinceTick();
  if (passed_milliseconds > timeout_ms_ && last_pose_ != last_debounced_pose_) {
    last_debounced_pose_ = last_pose_;
    last_pose_time_.tick();
    core::DeviceListenerWrapper::onPose(myo, 0, Pose(last_pose_));
  }
  core::DeviceListenerWrapper::onPeriodic(myo);
}

template <class ParentFeature>
Debounce<ParentFeature> make_debounce(ParentFeature& parent_feature,
                                      int timeout_ms = 10) {
  return Debounce<ParentFeature>(parent_feature, timeout_ms);
}
}
}
