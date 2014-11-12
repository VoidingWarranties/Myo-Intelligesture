/* DeviceListenerWrapper wraps the myo::DeviceListener class in order to add the
 * virtual function onPeriodic(myo::Myo*) in order to allow the derived classes
 * to call Base::onPeriodic.
 */

#ifndef MYO_INTELLIGESTURE_DEVICELISTENERWRAPPER_H_
#define MYO_INTELLIGESTURE_DEVICELISTENERWRAPPER_H_

#include <myo/myo.hpp>

class DeviceListenerWrapper : public myo::DeviceListener {
 public:
  virtual void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    onPose(myo, pose);
  }
  virtual void onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                 const myo::Quaternion<float>& quat) {}

  virtual void onPose(myo::Myo* myo, myo::Pose pose) = 0;
  virtual void onPeriodic(myo::Myo* myo) {}
};

#endif
