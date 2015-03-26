#include <string>
#include <sstream>
#include "../src/core/DeviceListenerWrapper.h"

#define PRINT_NAME_AND_VAR(x) " "#x": " << x

namespace {
std::ostream& operator<<(std::ostream& out,
                         myo::FirmwareVersion firmware_version) {
  out << "(" << firmware_version.firmwareVersionMajor;
  out << ", " << firmware_version.firmwareVersionMinor;
  out << ", " << firmware_version.firmwareVersionPatch;
  out << ", " << firmware_version.firmwareVersionHardwareRev << ")";
  return out;
}

std::ostream& operator<<(std::ostream& out,
                         const myo::Quaternion<float>& quat) {
  out << "(" << quat.x();
  out << ", " << quat.y();
  out << ", " << quat.z();
  out << ", " << quat.w() << ")";
  return out;
}

std::ostream& operator<<(std::ostream& out, const myo::Vector3<float>& quat) {
  out << "(" << quat.x();
  out << ", " << quat.y();
  out << ", " << quat.z() << ")";
  return out;
}

std::ostream& operator<<(std::ostream& out, myo::Arm arm) {
  switch (arm) {
    case myo::armLeft:
      out << "armLeft";
      break;
    case myo::armRight:
      out << "armRight";
      break;
    case myo::armUnknown:
      out << "armUnknown";
      break;
    default:
      out << "unknown";
      break;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, myo::XDirection x_direction) {
  switch (x_direction) {
    case myo::xDirectionTowardWrist:
      out << "xDirectionTowardWrist";
      break;
    case myo::xDirectionTowardElbow:
      out << "xDirectionTowardElbow";
      break;
    case myo::xDirectionUnknown:
      out << "xDirectionUnknown";
      break;
    default:
      out << "unknown";
      break;
  }
  return out;
}
}

class PrintEvents : public core::DeviceListenerWrapper {
 public:
  PrintEvents(core::DeviceListenerWrapper& parent_feature, std::string& out);

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
  std::string& out_;
};

PrintEvents::PrintEvents(core::DeviceListenerWrapper& parent_feature,
                         std::string& out)
    : out_(out) {
  parent_feature.addChildFeature(this);
}

void PrintEvents::onPair(myo::Myo* myo, uint64_t timestamp,
                         myo::FirmwareVersion firmwareVersion) {
  std::stringstream ss;
  ss << "onPair -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(firmwareVersion);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onUnpair(myo::Myo* myo, uint64_t timestamp) {
  std::stringstream ss;
  ss << "onUnpair -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onConnect(myo::Myo* myo, uint64_t timestamp,
                            myo::FirmwareVersion firmwareVersion) {
  std::stringstream ss;
  ss << "onConnect -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(firmwareVersion);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onDisconnect(myo::Myo* myo, uint64_t timestamp) {
  std::stringstream ss;
  ss << "onDisconnect -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onArmSync(myo::Myo* myo, uint64_t timestamp, myo::Arm arm,
                            myo::XDirection xDirection) {
  std::stringstream ss;
  ss << "onArmSync -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(arm);
  ss << PRINT_NAME_AND_VAR(xDirection);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onArmUnsync(myo::Myo* myo, uint64_t timestamp) {
  std::stringstream ss;
  ss << "onArmUnsync -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onUnlock(myo::Myo* myo, uint64_t timestamp) {
  std::stringstream ss;
  ss << "onUnlock -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onLock(myo::Myo* myo, uint64_t timestamp) {
  std::stringstream ss;
  ss << "onLock -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onPose(myo::Myo* myo, uint64_t timestamp,
                         const std::shared_ptr<core::Pose>& pose) {
  std::stringstream ss;
  ss << "onPose -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(*pose);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onGesture(myo::Myo* myo, uint64_t timestamp,
                            const std::shared_ptr<core::Gesture>& gesture) {
  std::stringstream ss;
  ss << "onGesture -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(*gesture);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onOrientationData(myo::Myo* myo, uint64_t timestamp,
                                    const myo::Quaternion<float>& rotation) {
  std::stringstream ss;
  ss << "onOrientationData -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(rotation);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onAccelerometerData(myo::Myo* myo, uint64_t timestamp,
                                      const myo::Vector3<float>& accel) {
  std::stringstream ss;
  ss << "onAccelerometerData -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(accel);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onGyroscopeData(myo::Myo* myo, uint64_t timestamp,
                                  const myo::Vector3<float>& gyro) {
  std::stringstream ss;
  ss << "onGyroscopeData -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(gyro);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onRssi(myo::Myo* myo, uint64_t timestamp, int8_t rssi) {
  std::stringstream ss;
  ss << "onRssi -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << PRINT_NAME_AND_VAR(rssi);
  ss << "\n";
  out_ += ss.str();
}

void PrintEvents::onEmgData(myo::Myo* myo, uint64_t timestamp,
                            const int8_t* emg) {
  std::stringstream ss;
  ss << "onAccelerometerData -";
  ss << PRINT_NAME_AND_VAR(myo);
  ss << PRINT_NAME_AND_VAR(timestamp);
  ss << "emg: (";
  ss << emg[0];
  for (std::size_t i = 1; i < 8; ++i) {
    ss << ", " << emg[i];
  }
  ss << ")\n";
  out_ += ss.str();
}
