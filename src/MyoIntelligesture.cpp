#include <iostream>
#include <myo/myo.hpp>

#include "DeviceListenerWrapper.h"
#include "OrientationPoses.h"
#include "Debounce.h"
#include "Orientation.h"
#include "PoseGestures.h"

int main() {
  try {
    myo::Hub hub("com.voidingwarranties.myo-intelligesture");
    myo::Myo* myo = hub.waitForMyo(10000);
    if (!myo) {
      throw std::runtime_error("Unable to find a Myo!");
    }

    PoseGestures<> listener;

    hub.addListener(&listener);

    // Event loop.
    while (true) {
      hub.run(1000 / 20);
      listener.onPeriodic(myo);
    }
  } catch (const std::exception& ex) {
    std::cerr << "Error: " << ex.what() << std::endl;
    return 1;
  }
  return 0;
}
