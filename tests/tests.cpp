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

// Test without MyoSimulator.
void testRootFeature();
void testDebounce();
void testExponentialMovingAverage();
void testMovingAverage();

// Test using MyoSimulator.
void myoSimTestRootFeature(MyoSim::Hub& hub);
void myoSimTestDebounce(MyoSim::Hub& hub);
void myoSimTestExponentialMovingAverage(MyoSim::Hub& hub);
void myoSimTestMovingAverage(MyoSim::Hub& hub);

int main() {
  // Test without MyoSimulator.
  testRootFeature();
  testDebounce();
  testExponentialMovingAverage();
  testMovingAverage();

  // Test using MyoSimulator.
  MyoSim::Hub hub("com.voidingwarranties.myo-intelligesture-tests");
  myoSimTestRootFeature(hub);
  myoSimTestDebounce(hub);
  myoSimTestExponentialMovingAverage(hub);
  myoSimTestMovingAverage(hub);

  return 0;
}

////////////////////////////////
// Tests without MyoSimulator //
////////////////////////////////

void testRootFeature() {
  features::RootFeature root_feature;
  std::string str;
  PrintEvents print_events(root_feature, str);

  uint64_t timestamp = 0;
  root_feature.onPair(nullptr, timestamp++, myo::FirmwareVersion{0, 1, 2, 3});
  root_feature.onUnpair(nullptr, timestamp++);
  root_feature.onConnect(nullptr, timestamp++, myo::FirmwareVersion{0, 1, 2, 3});
  root_feature.onDisconnect(nullptr, timestamp++);
  root_feature.onArmSync(nullptr, timestamp++, myo::armLeft, myo::xDirectionTowardWrist);
  root_feature.onArmUnsync(nullptr, timestamp++);
  root_feature.onUnlock(nullptr, timestamp++);
  root_feature.onLock(nullptr, timestamp++);
  root_feature.onPose(nullptr, timestamp++, myo::Pose::rest);
  root_feature.onGesture(nullptr, timestamp++, std::make_shared<core::Gesture>(core::Gesture::unknown));
  root_feature.onOrientationData(nullptr, timestamp++, myo::Quaternion<float>(0.f, 1.f, 2.f, 3.f));
  root_feature.onAccelerometerData(nullptr, timestamp++, myo::Vector3<float>(0.f, 1.f, 2.f));
  root_feature.onGyroscopeData(nullptr, timestamp++, myo::Vector3<float>(0.f, 1.f, 2.f));
  root_feature.onRssi(nullptr, timestamp++, 123);
  std::array<int8_t, 8> emg_data{{0, 1, 2, 3, 4, 5, 6, 7}};
  root_feature.onEmgData(nullptr, timestamp++, emg_data.data());
  root_feature.onPeriodic(nullptr);

  assert(str ==
         "onPair - myo: 0x0 timestamp: 0 firmwareVersion: (0, 1, 2, 3)\n"
         "onUnpair - myo: 0x0 timestamp: 1\n"
         "onConnect - myo: 0x0 timestamp: 2 firmwareVersion: (0, 1, 2, 3)\n"
         "onDisconnect - myo: 0x0 timestamp: 3\n"
         "onArmSync - myo: 0x0 timestamp: 4 arm: armLeft xDirection: xDirectionTowardWrist\n"
         "onArmUnsync - myo: 0x0 timestamp: 5\n"
         "onUnlock - myo: 0x0 timestamp: 6\n"
         "onLock - myo: 0x0 timestamp: 7\n"
         "onPose - myo: 0x0 timestamp: 8 *pose: rest\n"
         "onGesture - myo: 0x0 timestamp: 9 *gesture: unknown\n"
         "onOrientationData - myo: 0x0 timestamp: 10 rotation: (0, 1, 2, 3)\n"
         "onAccelerometerData - myo: 0x0 timestamp: 11 accel: (0, 1, 2)\n"
         "onGyroscopeData - myo: 0x0 timestamp: 12 gyro: (0, 1, 2)\n"
         "onRssi - myo: 0x0 timestamp: 13 rssi: 123\n"
         "onEmgData - myo: 0x0 timestamp: 14 emg: (0, 1, 2, 3, 4, 5, 6, 7)\n"
         "onPeriodic - myo: 0x0\n");
}

void testDebounce() {
  for (int debounce_ms : {5, 10, 100}) {
    auto test_debounce = [debounce_ms](int timestamp_offset) {
      features::RootFeature root_feature;
      features::filters::Debounce debounce(root_feature, debounce_ms);
      std::string str;
      PrintEvents print_events(debounce, str);

      uint64_t timestamp = 0;
      debounce.onPose(nullptr, timestamp++,
                      std::make_shared<core::Pose>(myo::Pose::rest));
      debounce.onPose(nullptr, timestamp++,
                      std::make_shared<core::Pose>(myo::Pose::fist));
      debounce.onPose(nullptr, timestamp++ - 1 + timestamp_offset,
                      std::make_shared<core::Pose>(myo::Pose::rest));

      return str;
    };

    std::string result;
    result = test_debounce(debounce_ms * 1000 + 1);
    assert(result == "onPose - myo: 0x0 timestamp: 1 *pose: fist\n");

    result = test_debounce(debounce_ms * 1000 - 1);
    assert(result == "");
  }
}

void testExponentialMovingAverage() {
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

    uint64_t timestamp = 0;
    for (std::size_t i = 0; i < 3; ++i) {
      avg.onOrientationData(nullptr, timestamp++,
                            myo::Quaternion<float>(i, i, i, i));
      avg.onAccelerometerData(nullptr, timestamp++,
                              myo::Vector3<float>(i, i, i));
      avg.onGyroscopeData(nullptr, timestamp++, myo::Vector3<float>(i, i, i));
    }

    assert (str == alpha.second);
  }
}

void testMovingAverage() {
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

    uint64_t timestamp = 0;
    for (std::size_t i = 0; i < 3; ++i) {
      avg.onOrientationData(nullptr, timestamp++,
                            myo::Quaternion<float>(i, i, i, i));
      avg.onAccelerometerData(nullptr, timestamp++,
                              myo::Vector3<float>(i, i, i));
      avg.onGyroscopeData(nullptr, timestamp++, myo::Vector3<float>(i, i, i));
    }

    assert (str == window_size.second);
  }
}

//////////////////////////////
// Tests using MyoSimulator //
//////////////////////////////

void myoSimTestRootFeature(MyoSim::Hub& hub) {
  features::RootFeature root_feature;
  std::string str;
  PrintEvents print_events(root_feature, str);
  hub.addListener(&root_feature);

  uint64_t timestamp = 0;
  MyoSim::EventLoopGroup elg;
  elg.group.push_back(std::make_shared<MyoSim::onPairEvent>(
      0, timestamp++, myo::FirmwareVersion{0, 1, 2, 3}));
  elg.group.push_back(std::make_shared<MyoSim::onUnpairEvent>(
      0, timestamp++));
  elg.group.push_back(std::make_shared<MyoSim::onConnectEvent>(
      0, timestamp++, myo::FirmwareVersion{0, 1, 2, 3}));
  elg.group.push_back(std::make_shared<MyoSim::onDisconnectEvent>(
      0, timestamp++));
  elg.group.push_back(std::make_shared<MyoSim::onArmSyncEvent>(
      0, timestamp++, myo::armLeft, myo::xDirectionTowardWrist));
  elg.group.push_back(std::make_shared<MyoSim::onArmUnsyncEvent>(
      0, timestamp++));
  elg.group.push_back(std::make_shared<MyoSim::onUnlockEvent>(
      0, timestamp++));
  elg.group.push_back(std::make_shared<MyoSim::onLockEvent>(
      0, timestamp++));
  elg.group.push_back(std::make_shared<MyoSim::onPoseEvent>(
      0, timestamp++, myo::Pose::rest));
  elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
      0, timestamp++, myo::Quaternion<float>(0.f, 1.f, 2.f, 3.f)));
  elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
      0, timestamp++, myo::Vector3<float>(0.f, 1.f, 2.f)));
  elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
      0, timestamp++, myo::Vector3<float>(0.f, 1.f, 2.f)));
  elg.group.push_back(std::make_shared<MyoSim::onRssiEvent>(
      0, timestamp++, 123));
  std::array<int8_t, 8> emg_data{{0, 1, 2, 3, 4, 5, 6, 7}};
  elg.group.push_back(std::make_shared<MyoSim::onEmgDataEvent>(
      0, timestamp++, emg_data.data()));
  MyoSim::EventSession event_session;
  event_session.events.push_back(elg);

  MyoSim::EventPlayer event_player(hub);
  event_player.play(event_session);
  hub.removeListener(&root_feature);

  assert(str ==
         "onPair - myo: 0x0 timestamp: 0 firmwareVersion: (0, 1, 2, 3)\n"
         "onUnpair - myo: 0x0 timestamp: 1\n"
         "onConnect - myo: 0x0 timestamp: 2 firmwareVersion: (0, 1, 2, 3)\n"
         "onDisconnect - myo: 0x0 timestamp: 3\n"
         "onArmSync - myo: 0x0 timestamp: 4 arm: armLeft xDirection: xDirectionTowardWrist\n"
         "onArmUnsync - myo: 0x0 timestamp: 5\n"
         "onUnlock - myo: 0x0 timestamp: 6\n"
         "onLock - myo: 0x0 timestamp: 7\n"
         "onPose - myo: 0x0 timestamp: 8 *pose: rest\n"
         "onOrientationData - myo: 0x0 timestamp: 9 rotation: (0, 1, 2, 3)\n"
         "onAccelerometerData - myo: 0x0 timestamp: 10 accel: (0, 1, 2)\n"
         "onGyroscopeData - myo: 0x0 timestamp: 11 gyro: (0, 1, 2)\n"
         "onRssi - myo: 0x0 timestamp: 12 rssi: 123\n"
         "onEmgData - myo: 0x0 timestamp: 13 emg: (0, 1, 2, 3, 4, 5, 6, 7)\n");
}

void myoSimTestDebounce(MyoSim::Hub& hub) {
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

void myoSimTestExponentialMovingAverage(MyoSim::Hub& hub) {
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
        0, timestamp++, myo::Quaternion<float>(0, 0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        0, timestamp++, myo::Vector3<float>(0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        0, timestamp++, myo::Vector3<float>(0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        0, timestamp++, myo::Quaternion<float>(1, 1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        0, timestamp++, myo::Vector3<float>(1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        0, timestamp++, myo::Vector3<float>(1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        0, timestamp++, myo::Quaternion<float>(2, 2, 2, 2)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        0, timestamp++, myo::Vector3<float>(2, 2, 2)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        0, timestamp++, myo::Vector3<float>(2, 2, 2)));
    MyoSim::EventSession event_session;
    event_session.events.push_back(elg);

    MyoSim::EventPlayer event_player(hub);
    event_player.play(event_session);
    hub.removeListener(&root_feature);

    assert (str == alpha.second);
  }
}

void myoSimTestMovingAverage(MyoSim::Hub& hub) {
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
        0, timestamp++, myo::Quaternion<float>(0, 0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        0, timestamp++, myo::Vector3<float>(0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        0, timestamp++, myo::Vector3<float>(0, 0, 0)));
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        0, timestamp++, myo::Quaternion<float>(1, 1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        0, timestamp++, myo::Vector3<float>(1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        0, timestamp++, myo::Vector3<float>(1, 1, 1)));
    elg.group.push_back(std::make_shared<MyoSim::onOrientationDataEvent>(
        0, timestamp++, myo::Quaternion<float>(2, 2, 2, 2)));
    elg.group.push_back(std::make_shared<MyoSim::onAccelerometerDataEvent>(
        0, timestamp++, myo::Vector3<float>(2, 2, 2)));
    elg.group.push_back(std::make_shared<MyoSim::onGyroscopeDataEvent>(
        0, timestamp++, myo::Vector3<float>(2, 2, 2)));
    MyoSim::EventSession event_session;
    event_session.events.push_back(elg);

    MyoSim::EventPlayer event_player(hub);
    event_player.play(event_session);
    hub.removeListener(&root_feature);

    assert (str == window_size.second);
  }
}
