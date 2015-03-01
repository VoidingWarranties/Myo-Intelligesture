#include <iostream>
#include <myo/myo.hpp>

#include "RootFeature.h"
#include "Debounce.h"
#include "Orientation.h"
#include "OrientationPoses.h"
#include "PoseGestures.h"
#include "ExampleClass.h"

int main() {
  try {
    myo::Hub hub("com.voidingwarranties.myo-intelligesture");
    myo::Myo* myo = hub.waitForMyo(10000);
    if (!myo) {
      throw std::runtime_error("Unable to find a Myo!");
    }

    RootFeature root_feature;
    auto debounce = make_debounce(root_feature);
    auto orientation = make_orientation(root_feature);
    auto orientation_poses = make_orientation_poses(debounce, orientation);
    auto pose_gestures = make_pose_gestures(orientation_poses);
    auto example = make_example(pose_gestures, orientation);

    hub.addListener(&root_feature);

    // Event loop.
    while (true) {
      myo->unlock(myo::Myo::unlockTimed);
      hub.run(1000 / 20);
      root_feature.onPeriodic(myo);
    }
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
