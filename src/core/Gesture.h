#pragma once

#include <string>

#include "Pose.h"

namespace core {
class Gesture {
 public:
  enum Type { unkown };

  Gesture(Type type = unkown);
  Gesture(const std::shared_ptr<Pose>& pose, Type type = unkown);
  virtual ~Gesture() {}

  bool operator==(const Gesture& gesture);
  bool operator!=(const Gesture& gesture);

  virtual std::string toString() const;

  std::shared_ptr<Pose> AssociatedPose() const;

  std::string toDescriptiveString() const;

 protected:
  std::shared_ptr<Pose> associated_pose_;

 private:
  Type type_;
};

Gesture::Gesture(Type type)
    : type_(type), associated_pose_(new Pose(Pose::rest)) {}

Gesture::Gesture(const std::shared_ptr<Pose>& pose, Type type)
    : type_(type), associated_pose_(pose) {}

bool Gesture::operator==(const Gesture& gesture) {
  return this->toString() == gesture.toString();
}

bool Gesture::operator!=(const Gesture& gesture) {
  return this->toString() != gesture.toString();
}

std::string Gesture::toString() const { return "unkown"; }

std::shared_ptr<Pose> Gesture::AssociatedPose() const {
  return associated_pose_;
}

std::string Gesture::toDescriptiveString() const {
  return this->toString() + ": " + associated_pose_->toString();
}

std::ostream& operator<<(std::ostream& os, const Gesture& gesture) {
  return os << gesture.toString();
}
}
