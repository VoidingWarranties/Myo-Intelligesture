#pragma once

#include <string>
#include <iostream>

namespace core {
class Pose {
 public:
  enum Type { rest, fist, waveIn, waveOut, fingersSpread, doubleTap, unknown };

  Pose(Type type = unknown);
  Pose(const myo::Pose& pose);
  virtual ~Pose() {}

  bool operator==(const Pose& pose) const;
  bool operator!=(const Pose& pose) const;

  virtual std::string toString() const;

 private:
  Type type_;
};

Pose::Pose(Type type) : type_(type) {}

Pose::Pose(const myo::Pose& pose) {
  switch (pose.type()) {
    case myo::Pose::rest:
      type_ = rest;
      break;
    case myo::Pose::fist:
      type_ = fist;
      break;
    case myo::Pose::waveIn:
      type_ = waveIn;
      break;
    case myo::Pose::waveOut:
      type_ = waveOut;
      break;
    case myo::Pose::fingersSpread:
      type_ = fingersSpread;
      break;
    case myo::Pose::doubleTap:
      type_ = doubleTap;
      break;
    default:
      type_ = unknown;
      break;
  }
}

// Use toString for comparisons so the operator can be used polymorphically.
// Because of this toString MUST return a unique value.
bool Pose::operator==(const Pose& pose) const {
  return this->toString() == pose.toString();
}

bool Pose::operator!=(const Pose& pose) const {
  return this->toString() != pose.toString();
}

std::string Pose::toString() const {
  switch (type_) {
    case rest:
      return "rest";
    case fist:
      return "fist";
    case waveIn:
      return "waveIn";
    case waveOut:
      return "waveOut";
    case fingersSpread:
      return "fingersSpread";
    case doubleTap:
      return "doubleTap";
    default:
      return "unknown";
  }
}

std::ostream& operator<<(std::ostream& os, const Pose& pose) {
  return os << pose.toString();
}
}
