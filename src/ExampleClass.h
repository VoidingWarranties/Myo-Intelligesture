#ifndef MYO_INTELLIGESTURE_EXAMPLECLASS_H_
#define MYO_INTELLIGESTURE_EXAMPLECLASS_H_

#include <myo/myo.hpp>
#include "PoseGestures.h"

template <class BaseClass = PoseGestures<>, class PoseClass = PoseGestures<>::Pose>
class ExampleClass : public BaseClass {
 public:
  void onPose(myo::Myo* myo, PoseClass pose) {
    std::cout << pose << std::endl;
  }
};

#endif
