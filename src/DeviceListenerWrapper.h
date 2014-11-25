/* DeviceListenerWrapper wraps the myo::DeviceListener class in order to add the
 * virtual function onPeriodic(myo::Myo*) in order to allow the derived classes
 * to call Base::onPeriodic.
 */

#ifndef MYO_INTELLIGESTURE_DEVICE_LISTENER_WRAPPER_H_
#define MYO_INTELLIGESTURE_DEVICE_LISTENER_WRAPPER_H_

#include <myo/myo.hpp>

template <class PoseClass>
class DeviceListenerWrapper : public myo::DeviceListener {
 public:
  virtual void onPose(myo::Myo*, uint64_t, PoseClass) {}
  virtual void onPeriodic(myo::Myo* myo) {}
};

#endif
