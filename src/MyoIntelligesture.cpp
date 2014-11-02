#include <iostream>

#include "AtomicEventListener.h"
#include "Debounce.h"
#include "PoseGestures.h"
#include "PosePatterns.h"

#include <myo/myo.hpp>

int main() {
  try {
    myo::Hub hub("com.voidingwarranties.myo-intelligesture");
    myo::Myo* myo = hub.waitForMyo(10000);
    if (!myo) {
      throw std::runtime_error("Unable to find a Myo!");
    }

    PosePatterns pose_patts(PosePatterns::SUGGESTED_MAX_DELAY,
                            [](myo::Myo* myo, uint64_t timestamp,
                               myo::Pose pose, PosePatterns::Pattern pattern) {
                              std::cout << pattern << ": " << pose << std::endl;
                            },
                            [](myo::Myo* myo) {});

    PoseGestures pose_gests(
        PoseGestures::SUGGESTED_MAX_CLICK_TIME,
        PoseGestures::SUGGESTED_HOLD_TIME,
        [&pose_patts](myo::Myo* myo, uint64_t timestamp, myo::Pose pose,
                      PoseGestures::Gesture gesture) {
          pose_patts.onPose(myo, timestamp, pose, gesture);
        },
        [&pose_patts](myo::Myo* myo) { pose_patts.onPeriodic(myo); });

    Debounce debounce(
        Debounce::SUGGESTED_DEBOUNCE_DELAY,
        [&pose_gests](myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
          pose_gests.onPose(myo, timestamp, pose);
        },
        [&pose_gests](myo::Myo* myo) { pose_gests.onPeriodic(myo); });

    AtomicEventListener listener(
        [&debounce](myo::Myo* myo, uint64_t timestamp,
                    myo::Pose pose) { debounce.onPose(myo, timestamp, pose); },
        [&debounce](myo::Myo* myo) { debounce.onPeriodic(myo); });

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
