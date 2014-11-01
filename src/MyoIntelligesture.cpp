#include <iostream>

#include "AtomicEventListener.h"

#include <myo/myo.hpp>

int main() {
  try {
    myo::Hub hub("com.voidingwarranties.myo-intelligesture");
    myo::Myo* myo = hub.waitForMyo(10000);
    if (!myo) {
      throw std::runtime_error("Unable to find a Myo!");
    }

    AtomicEventListener listener(
        [](myo::Myo* myo, uint64_t timestamp,
           myo::Pose pose) { std::cout << "pose: " << pose << std::endl; },
        [](myo::Myo* myo) {});
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
}
