#include <myo/myo.hpp>
#include <sstream>

#include "../src/core/DeviceListenerWrapper.h"
#include "../src/features/RootFeature.h"
#include "../src/features/filters/Debounce.h"

#include "../lib/MyoSimulator/src/Hub.h"
#include "../lib/MyoSimulator/src/EventTypes.h"
#include "../lib/MyoSimulator/src/EventPlayer.h"

#include "PrintEventsFeature.h"

void testDebounce();

int main() {
  testDebounce();

  return 0;
}

void testDebounce() {
  for (int debounce_ms : {5, 10, 100}) {
    auto test_debounce = [debounce_ms](int timestamp_offset) {
      std::string str;
      features::RootFeature root_feature;
      features::filters::Debounce debounce(root_feature, debounce_ms);
      PrintEvents print_events(debounce, str);

      MyoSim::Hub hub("com.voidingwarranties.debounce-test");
      hub.addListener(&root_feature);

      MyoSim::EventLoopGroup elg;
      elg.group.push_back(
          std::make_shared<MyoSim::onPoseEvent>(0, 0, myo::Pose::rest));
      elg.group.push_back(
          std::make_shared<MyoSim::onPoseEvent>(0, 1, myo::Pose::fist));
      elg.group.push_back(std::make_shared<MyoSim::onPoseEvent>(
          0, 1 + timestamp_offset, myo::Pose::rest));
      MyoSim::EventSession event_session;
      event_session.events.push_back(elg);

      MyoSim::EventPlayer event_player(hub);
      event_player.play(event_session, 1,
                        [&]() { root_feature.onPeriodic(0); });
      return str;
    };

    std::string result;
    result = test_debounce(debounce_ms * 1000 + 1);
    assert(result == "onPose - myo: 0x0 timestamp: 1 *pose: fist\n");

    result = test_debounce(debounce_ms * 1000 - 1);
    assert(result == "");
  }
}
