#include <iostream>
#include <myo/myo.hpp>

#include "../src/features/RootFeature.h"
#include "../src/features/filters/Debounce.h"
#include "../src/features/Orientation.h"
#include "../src/features/OrientationPoses.h"
#include "../src/features/filters/ExponentialMovingAverage.h"
#include "../src/features/filters/MovingAverage.h"
#include "ExampleFeature.h"

int main() {
  try {
    myo::Hub hub("com.voidingwarranties.myo-intelligesture");
    myo::Myo* myo = hub.waitForMyo(10000);
    if (!myo) {
      throw std::runtime_error("Unable to find a Myo!");
    }

    features::RootFeature root_feature;
    auto debounce = features::filters::make_debounce(root_feature);
    // This filter averages only orientation data.
    auto moving_average = features::filters::make_moving_average(
        root_feature, features::filters::MovingAverage::OrientationData, 10);
    // This filter averages only accelerometer and gyroscope data.
    auto exponential_moving_average =
        features::filters::make_exponential_moving_average(
            moving_average,
            features::filters::ExponentialMovingAverage::AccelerometerData |
                features::filters::ExponentialMovingAverage::GyroscopeData,
            0.2);
    auto orientation = features::make_orientation(exponential_moving_average);
    auto orientation_poses =
        features::make_orientation_poses(debounce, orientation);
    auto example = make_example(orientation_poses, orientation);

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
