/* Debounce class debounces poses to reduce accidental poses. Only poses held
 * for at least the debounce delay will trigger a pose. A pose which is held for
 * longer than the debounce delay will trigger the virtual function
 * onPose(myo::Myo*, Pose).
 */

#ifndef MYO_INTELLIGESTURE_DEBOUNCE_H_
#define MYO_INTELLIGESTURE_DEBOUNCE_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"
#include "../../Basic-Timer/BasicTimer.h"

template <class ParentFeature>
class Debounce : public DeviceListenerWrapper<typename ParentFeature::Pose> {
  typedef typename ParentFeature::Pose ParentPose;
  typedef DeviceListenerWrapper<ParentPose> BaseClass;

 public:
  typedef ParentPose Pose;

  Debounce(ParentFeature& parent_feature, int timeout_ms = 10)
      : timeout_ms_(timeout_ms),
        last_pose_(ParentPose::rest),
        last_debounced_pose_(last_pose_) {
    parent_feature.addChildFeature(this);
    last_pose_time_.tick();
  }

  virtual void onPose(myo::Myo* myo, uint64_t timestamp, ParentPose pose) {
    last_pose_ = pose;
    last_pose_time_.tick();
  }

  virtual void onPeriodic(myo::Myo* myo) {
    uint64_t passed_milliseconds = last_pose_time_.millisecondsSinceTick();
    if (passed_milliseconds > timeout_ms_ &&
        last_pose_ != last_debounced_pose_) {
      last_debounced_pose_ = last_pose_;
      last_pose_time_.tick();
      BaseClass::onPose(myo, 0, Pose(last_pose_));
    }

    BaseClass::onPeriodic(myo);
  }

 private:
  int timeout_ms_;
  ParentPose last_pose_, last_debounced_pose_;
  BasicTimer last_pose_time_;
};

template <class ParentFeature>
Debounce<ParentFeature> make_debounce(ParentFeature& parent_feature) {
  return Debounce<ParentFeature>(parent_feature);
}

#endif
