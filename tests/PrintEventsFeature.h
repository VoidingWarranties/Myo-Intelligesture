#include <ostream>
#include "../src/core/DeviceListenerWrapper.h"

#define PRINT_NAME_AND_VAR(x) " "#x": " << x

namespace {
std::ostream& operator<<(std::ostream& out,
                         myo::FirmwareVersion firmware_version) {
  out << "(" << firmware_version.firmwareVersionMajor;
  out << ", " << firmware_version.firmwareVersionMinor;
  out << ", " << firmware_version.firmwareVersionPatch;
  out << ", " << firmware_version.firmwareVersionHardwareRev << ")";
}

std::ostream& operator<<(std::ostream& out, const myo::Quaternion<float>& quat) {
  out << "(" << quat.x();
  out << ", " << quat.y();
  out << ", " << quat.z();
  out << ", " << quat.w() << ")";
}

std::ostream& operator<<(std::ostream& out, const myo::Vector3<float>& quat) {
  out << "(" << quat.x();
  out << ", " << quat.y();
  out << ", " << quat.z() << ")";
}
}

class PrintEvents : public core::DeviceListenerWrapper {
 public:
  PrintEvents(core::DeviceListenerWrapper& parent_feature, std::ostream& out);

  virtual void onPair(myo::Myo* myo, uint64_t timestamp,
                      myo::FirmwareVersion firmwareVersion) override;
  virtual void onUnpair(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onConnect(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmwareVersion) override;
  virtual void onDisconnect(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                         myo::XDirection xDirection) override;
  virtual void onArmUnsync(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onUnlock(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onLock(myo::Myo* myo, uint64_t timestamp) override;
  virtual void onPose(myo::Myo* myo, uint64_t timestamp,
                      const std::shared_ptr<core::Pose>& pose) override;
  virtual void onGesture(
      myo::Myo* myo, uint64_t timestamp,
      const std::shared_ptr<core::Gesture>& gesture) override;
  virtual void onOrientationData(
      myo::Myo* myo, uint64_t timestamp,
      const myo::Quaternion<float>& rotation) override;
  virtual void onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                   const myo::Vector3<float>& accel) override;
  virtual void onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                               const myo::Vector3<float>& gyro) override;
  virtual void onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) override;
  virtual void onEmgData(myo::Myo* myo, uint64_t timestamp,
                         const int8_t* emg) override;

 private:
  std::ostream& out_;
};

PrintEvents::PrintEvents(core::DeviceListenerWrapper& parent_feature,
                         std::ostream& out)
    : out_(out) {
  parent_feature.addChildFeature(this);
}

void PrintEvents::onPair(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmwareVersion) {
  out_ << "onPair -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(firmwareVersion);
  out_ << "\n";
}

void PrintEvents::onUnpair(myo::Myo* myo, uint64_t timestamp) {
  out_ << "onUnpair -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << "\n";
}

void PrintEvents::onConnect(myo::Myo* myo, uint64_t timestamp,
                            myo::FirmwareVersion firmwareVersion) {
  out_ << "onConnect -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(firmwareVersion);
  out_ << "\n";
}

void PrintEvents::onDisconnect(myo::Myo* myo, uint64_t timestamp) {
  out_ << "onDisconnect -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << "\n";
}

void PrintEvents::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                            myo::XDirection xDirection) {
  out_ << "onArmSync -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(arm);
  out_ << PRINT_NAME_AND_VAR(xDirection);
  out_ << "\n";
}

void PrintEvents::onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
  out_ << "onArmUnsync -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << "\n";
}

void PrintEvents::onUnlock(myo::Myo* myo, uint64_t timestamp) {
  out_ << "onUnlock -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << "\n";
}

void PrintEvents::onLock(myo::Myo* myo, uint64_t timestamp) {
  out_ << "onLock -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << "\n";
}

void PrintEvents::onPose(myo::Myo* myo, uint64_t timestamp,
                         const std::shared_ptr<core::Pose>& pose) {
  out_ << "onPose -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(*pose);
  out_ << "\n";
}

void PrintEvents::onGesture(myo::Myo* myo, uint64_t timestamp,
                            const std::shared_ptr<core::Gesture>& gesture) {
  out_ << "onGesture -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(*gesture);
  out_ << "\n";
}

void PrintEvents::onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                    const myo::Quaternion<float>& rotation) {
  out_ << "onOrientationData -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(rotation);
  out_ << "\n";
}

void PrintEvents::onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                      const myo::Vector3<float>& accel) {
  out_ << "onAccelerometerData -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(accel);
  out_ << "\n";
}

void PrintEvents::onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                                  const myo::Vector3<float>& gyro) {
  out_ << "onGyroscopeData -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(gyro);
  out_ << "\n";
}

void PrintEvents::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
  out_ << "onRssi -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << PRINT_NAME_AND_VAR(rssi);
  out_ << "\n";
}

void PrintEvents::onEmgData(myo::Myo* myo, uint64_t timestamp,
                            const int8_t* emg) {
  out_ << "onAccelerometerData -";
  out_ << PRINT_NAME_AND_VAR(myo);
  out_ << PRINT_NAME_AND_VAR(timestamp);
  out_ << "emg: (";
  out_ << emg[0];
  for (std::size_t i = 1; i < 8; ++i) {
    out_ << ", " << emg[i];
  }
  out_ << ")\n";
}
