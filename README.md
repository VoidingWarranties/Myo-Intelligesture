Myo Intelligesture
==================

A library for Myo which allows more complex gestures
----------------------------------------------------

Basic Info
----------

This library expands upon the Myo library provided in the SDK to provide an easy
interface to extract meaningful data from the provided data. For example, the
Myo provides orientation data, but a lot of boiler plate code is required to
extract the relative roll/pitch/yaw angle between two orientations. This library
provides `OrientationUtility.h` to make this calculation easy.

In addition, we introduce two new poses, `waveUp` and `waveDown` which are triggered
when `waveIn` or `waveOut` are triggered and the user's wrist is oriented
appropriately. For example, when the users palm is facing down, `waveUp` is
triggered instead of `waveOut`, and `waveDown` is triggered instead of `waveIn`. The
opposite is true when the user's palm is facing up.

We also provide a class to easily detect basic patterns which we call gestures.
These gestures include clicking, double clicking, and holding a pose. You can
also use this library to debounce the pose input if you find you often trigger
poses accidentally.

Finally, the library is very flexible. Each class is templated and allows you to
mix and match parts of the library together if you only want to use specific
features.

Usage
-----

The easiest way to use this library is to create a new class and have it derive
from `PoseGestures`. This will include all of the features from the library. You
must also implement the onPose function. Note that this onPose function is
different than the onPose function called by the `DeviceListener` class in two
ways. 1: it does not have a timestamp and 2: it has a custom pose type. This
pose type *must* match the pose type from the parent class. You can implement
any of the virtual functions defined in DeviceListener, however you *must* call
the same function in the parent class. You are free to call it anywhere in the
function you like, but we suggest calling it first before anything else.
```c++
#include <myo/myo.hpp>
#include "PoseGestures.h"

class ExampleClass : public PoseGestures<> {
 public:
  // This function is called from the PoseGestures class.
  void onPose(myo::Myo* myo, PoseGestures<>::Pose pose) {
    std::cout << "Look at all these new poses! " << pose << std::endl;
    // Do stuff using all of MyoIntelligesture's cool features :)
  }

  // This function is called from the DeviceListener class.
  void onPose(myo::Myo* myo, uint64_t timestamp, myo::Pose pose) {
    PoseGestures<>::onPose(myo, timestamp, pose);
    // Do stuff using plain old poses...
  }
};
```
You will notice `onPose(myo::Myo*, PoseGestures<>::Pose)` gets called multiple
times for every time `onPose(myo::Myo*, uint64_t, myo::Pose)` is called. This is
because gesture data is contained in the `PoseGestures<>::Pose` object.
Therefore if you quickly click a pose, it will triger an onPose event once with
a `none` gesture, and once with a `singleClick` gesture.

Future Plans
------------

- More complex pattern matching, possible using machine learning.
- Ability to suppress poses if it's part of a more complicated pose.
