/* DeviceListenerWrapper wraps the myo::DeviceListener class in order to add the
 * virtual function onPeriodic(myo::Myo*) in order to allow the derived classes
 * to call Base::onPeriodic.
 */

#ifndef MYO_INTELLIGESTURE_DEVICE_LISTENER_WRAPPER_H_
#define MYO_INTELLIGESTURE_DEVICE_LISTENER_WRAPPER_H_

#include <set>
#include <algorithm>
#include <myo/myo.hpp>

template <class PoseClass>
class DeviceListenerWrapper : public myo::DeviceListener {
 protected:
  typedef DeviceListenerWrapper<PoseClass>* listener_t;
  std::set<listener_t> listeners_;

 public:
  void addListener(listener_t listener) { listeners_.insert(listener); }
  void removeListener(listener_t listener) { listeners_.erase(listener); }

  virtual void onPose(myo::Myo* myo, uint64_t timestamp, PoseClass pose) {
    std::for_each(listeners_.begin(), listeners_.end(),
                  [myo, timestamp, pose](listener_t listener) {
      listener->onPose(myo, timestamp, pose);
    });
  }
  virtual void onPeriodic(myo::Myo* myo) {
    std::for_each(listeners_.begin(), listeners_.end(),
                  [myo](listener_t listener) {
      listener->onPeriodic(myo);
    });
  }
};

#endif
