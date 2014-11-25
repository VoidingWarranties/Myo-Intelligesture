#ifndef MYO_INTELLIGESTURE_EXAMPLECLASS_H_
#define MYO_INTELLIGESTURE_EXAMPLECLASS_H_

#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"

template <class PrevClass>
class ExampleClass : public DeviceListenerWrapper<typename PrevClass::Pose> {
 public:
  ExampleClass(PrevClass& prev_class) { prev_class.addListener(this); }

  virtual void onPose(myo::Myo* myo, uint64_t timestamp, typename PrevClass::Pose pose) {
    std::cout << pose << std::endl;
  }
};

template <class PrevClass>
ExampleClass<PrevClass> make_example(PrevClass& prev_class) {
  return ExampleClass<PrevClass>(prev_class);
}

#endif
