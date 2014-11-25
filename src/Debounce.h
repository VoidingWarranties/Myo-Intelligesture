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

template <class PrevClass>
class Debounce : public DeviceListenerWrapper<typename PrevClass::Pose> {
 public:
  typedef typename PrevClass::Pose Pose;

  Debounce(PrevClass& prev_class, int timeout_ms = 10)
      : timeout_ms_(timeout_ms),
        last_pose_(PrevClass::Pose::rest),
        last_debounced_pose_(last_pose_) {
    prev_class.addListener(this);
    last_pose_time_.tick();
  }

  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      typename PrevClass::Pose pose) {
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
  typedef DeviceListenerWrapper<typename PrevClass::Pose> BaseClass;

  int timeout_ms_;
  typename PrevClass::Pose last_pose_, last_debounced_pose_;
  BasicTimer last_pose_time_;
};

template <class PrevClass>
Debounce<PrevClass> make_debounce(PrevClass& prev_class) {
  return Debounce<PrevClass>(prev_class);
}

#endif
