#ifndef MYO_INTELLIGESTURE_EXAMPLECLASS_H_
#define MYO_INTELLIGESTURE_EXAMPLECLASS_H_

#include <myo/myo.hpp>
#include "PoseGestures.h"

typedef PoseGestures<> BaseClass;
typedef PoseGestures<>::Pose PoseClass;
class ExampleClass : public BaseClass {
 public:
  void onPose(myo::Myo* myo, PoseClass pose) {
    std::cout << pose << std::endl;
    if (pose.pose() == PoseClass::doubleTap) {
      std::cout << "HA" << std::endl;
      this->calibrateOrientation();
    }
  }
};

#endif
