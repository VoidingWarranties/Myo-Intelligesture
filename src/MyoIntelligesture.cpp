#include <iostream>
#include <myo/myo.hpp>

#include "RootFeature.h"

int main() {
  try {
    myo::Hub hub("com.voidingwarranties.myo-intelligesture");
    myo::Myo* myo = hub.waitForMyo(10000);
    if (!myo) {
      throw std::runtime_error("Unable to find a Myo!");
    }

    RootFeature root_feature;

    hub.addListener(&root_feature);

    // Event loop.
    while (true) {
      hub.run(1000 / 20);
      root_feature.onPeriodic(myo);
    }
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
