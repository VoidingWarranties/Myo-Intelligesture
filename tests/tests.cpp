#include <myo/myo.hpp>
#include <sstream>

#include "../src/core/DeviceListenerWrapper.h"
#include "../src/features/RootFeature.h"
#include "../src/features/filters/Debounce.h"
#include "../src/features/filters/ExponentialMovingAverage.h"
#include "../src/features/filters/MovingAverage.h"

#include "../lib/MyoSimulator/src/Hub.h"
#include "../lib/MyoSimulator/src/EventTypes.h"
#include "../lib/MyoSimulator/src/EventPlayer.h"

#include "PrintEventsFeature.h"

void testDebounce(MyoSim::Hub& hub);
void testExponentialMovingAverage(MyoSim::Hub& hub);
void testMovingAverage(MyoSim::Hub& hub);

int main() {
  MyoSim::Hub hub("com.voidingwarranties.myo-intelligesture-tests");

  testDebounce(hub);
  testExponentialMovingAverage(hub);
  testMovingAverage(hub);

  return 0;
}

void testDebounce(MyoSim::Hub& hub) {
  for (int debounce_ms : {5, 10, 100}) {
    auto test_debounce = [&hub, debounce_ms](int timestamp_offset) {
      features::RootFeature root_feature;
      features::filters::Debounce debounce(root_feature, debounce_ms);
      std::string str;
      PrintEvents print_events(debounce, str);
      hub.addListener(&root_feature);

      uint64_t timestamp = 0;
      MyoSim::EventLoopGroup elg;
      elg.group.push_back(std::make_shared<MyoSim::onPoseEvent>(
          0, timestamp++, myo::Pose::rest));
      elg.group.push_back(std::make_shared<MyoSim::onPoseEvent>(
          0, timestamp++, myo::Pose::fist));
      elg.group.push_back(std::make_shared<MyoSim::onPoseEvent>(
          0, timestamp++ - 1 + timestamp_offset, myo::Pose::rest));
      MyoSim::EventSession event_session;
      event_session.events.push_back(elg);

      MyoSim::EventPlayer event_player(hub);
      event_player.play(event_session);
      hub.removeListener(&root_feature);
      return str;
    };

    std::string result;
    result = test_debounce(debounce_ms * 1000 + 1);
    assert(result == "onPose - myo: 0x0 timestamp: 1 *pose: fist\n");

    result = test_debounce(debounce_ms * 1000 - 1);
    assert(result == "");
  }
}

void testExponentialMovingAverage(MyoSim::Hub& hub) {
  using features::filters::ExponentialMovingAverage;
  std::map<float, std::string> expected_results;
  expected_results[1.f] =
      "onOrientationData - myo: 0x0 timestamp: 0 rotation: (0, 0, 0, 0)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 1 accel: (0, 0, 0)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 2 gyro: (0, 0, 0)\n"
      "onOrientationData - myo: 0x0 timestamp: 3 rotation: (1, 1, 1, 1)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 4 accel: (1, 1, 1)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 5 gyro: (1, 1, 1)\n"
      "onOrientationData - myo: 0x0 timestamp: 6 rotation: (2, 2, 2, 2)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 7 accel: (2, 2, 2)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 8 gyro: (2, 2, 2)\n";
  expected_results[0.5f] =
      "onOrientationData - myo: 0x0 timestamp: 0 rotation: (0, 0, 0, 0)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 1 accel: (0, 0, 0)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 2 gyro: (0, 0, 0)\n"
      "onOrientationData - myo: 0x0 timestamp: 3 rotation: (0.5, 0.5, 0.5, 0.5)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 4 accel: (0.5, 0.5, 0.5)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 5 gyro: (0.5, 0.5, 0.5)\n"
      "onOrientationData - myo: 0x0 timestamp: 6 rotation: (1.25, 1.25, 1.25, 1.25)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 7 accel: (1.25, 1.25, 1.25)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 8 gyro: (1.25, 1.25, 1.25)\n";
  expected_results[0.25f] =
      "onOrientationData - myo: 0x0 timestamp: 0 rotation: (0, 0, 0, 0)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 1 accel: (0, 0, 0)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 2 gyro: (0, 0, 0)\n"
      "onOrientationData - myo: 0x0 timestamp: 3 rotation: (0.25, 0.25, 0.25, 0.25)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 4 accel: (0.25, 0.25, 0.25)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 5 gyro: (0.25, 0.25, 0.25)\n"
      "onOrientationData - myo: 0x0 timestamp: 6 rotation: (0.6875, 0.6875, 0.6875, 0.6875)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 7 accel: (0.6875, 0.6875, 0.6875)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 8 gyro: (0.6875, 0.6875, 0.6875)\n";

  for (const auto& alpha : expected_results) {
    features::RootFeature root_feature;
    ExponentialMovingAverage avg(
        root_feature, ExponentialMovingAverage::OrientationData |
                      ExponentialMovingAverage::AccelerometerData |
                      ExponentialMovingAverage::GyroscopeData,
        alpha.first);
    std::string str;
    PrintEvents print_events(avg, str);
    hub.addListener(&root_feature);

    uint64_t timestamp = 0;
    MyoSim::EventLoopGroup elg;
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        timestamp++, 0, myo::Quaternion<float>(0, 0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        timestamp++, 1, myo::Vector3<float>(0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        timestamp++, 2, myo::Vector3<float>(0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        timestamp++, 3, myo::Quaternion<float>(1, 1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        timestamp++, 4, myo::Vector3<float>(1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        timestamp++, 5, myo::Vector3<float>(1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        timestamp++, 6, myo::Quaternion<float>(2, 2, 2, 2)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        timestamp++, 7, myo::Vector3<float>(2, 2, 2)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        timestamp++, 8, myo::Vector3<float>(2, 2, 2)));
    MyoSim::EventSession event_session;
    event_session.events.push_back(elg);

    MyoSim::EventPlayer event_player(hub);
    event_player.play(event_session);
    hub.removeListener(&root_feature);

    assert (str == alpha.second);
  }
}

void testMovingAverage(MyoSim::Hub& hub) {
  using features::filters::MovingAverage;
  std::map<int, std::string> expected_results;
  expected_results[1] =
      "onOrientationData - myo: 0x0 timestamp: 0 rotation: (0, 0, 0, 0)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 1 accel: (0, 0, 0)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 2 gyro: (0, 0, 0)\n"
      "onOrientationData - myo: 0x0 timestamp: 3 rotation: (1, 1, 1, 1)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 4 accel: (1, 1, 1)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 5 gyro: (1, 1, 1)\n"
      "onOrientationData - myo: 0x0 timestamp: 6 rotation: (2, 2, 2, 2)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 7 accel: (2, 2, 2)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 8 gyro: (2, 2, 2)\n";
  expected_results[2] =
      "onOrientationData - myo: 0x0 timestamp: 0 rotation: (0, 0, 0, 0)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 1 accel: (0, 0, 0)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 2 gyro: (0, 0, 0)\n"
      "onOrientationData - myo: 0x0 timestamp: 3 rotation: (0.5, 0.5, 0.5, 0.5)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 4 accel: (0.5, 0.5, 0.5)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 5 gyro: (0.5, 0.5, 0.5)\n"
      "onOrientationData - myo: 0x0 timestamp: 6 rotation: (1.5, 1.5, 1.5, 1.5)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 7 accel: (1.5, 1.5, 1.5)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 8 gyro: (1.5, 1.5, 1.5)\n";
  expected_results[3] =
      "onOrientationData - myo: 0x0 timestamp: 0 rotation: (0, 0, 0, 0)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 1 accel: (0, 0, 0)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 2 gyro: (0, 0, 0)\n"
      "onOrientationData - myo: 0x0 timestamp: 3 rotation: (0.5, 0.5, 0.5, 0.5)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 4 accel: (0.5, 0.5, 0.5)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 5 gyro: (0.5, 0.5, 0.5)\n"
      "onOrientationData - myo: 0x0 timestamp: 6 rotation: (1, 1, 1, 1)\n"
      "onAccelerometerData - myo: 0x0 timestamp: 7 accel: (1, 1, 1)\n"
      "onGyroscopeData - myo: 0x0 timestamp: 8 gyro: (1, 1, 1)\n";

  for (const auto& window_size : expected_results) {
    features::RootFeature root_feature;
    MovingAverage avg(root_feature, MovingAverage::OrientationData |
                                    MovingAverage::AccelerometerData |
                                    MovingAverage::GyroscopeData,
                      window_size.first);
    std::string str;
    PrintEvents print_events(avg, str);
    hub.addListener(&root_feature);

    uint64_t timestamp = 0;
    MyoSim::EventLoopGroup elg;
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        timestamp++, 0, myo::Quaternion<float>(0, 0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        timestamp++, 1, myo::Vector3<float>(0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        timestamp++, 2, myo::Vector3<float>(0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        timestamp++, 3, myo::Quaternion<float>(1, 1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        timestamp++, 4, myo::Vector3<float>(1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        timestamp++, 5, myo::Vector3<float>(1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        timestamp++, 6, myo::Quaternion<float>(2, 2, 2, 2)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        timestamp++, 7, myo::Vector3<float>(2, 2, 2)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        timestamp++, 8, myo::Vector3<float>(2, 2, 2)));
    MyoSim::EventSession event_session;
    event_session.events.push_back(elg);

    MyoSim::EventPlayer event_player(hub);
    event_player.play(event_session);
    hub.removeListener(&root_feature);

    assert (str == window_size.second);
  }
}
