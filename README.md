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

In addition, we introduce two new poses, `waveUp` and `waveDown` which are
triggered when `waveIn` or `waveOut` are triggered and the user's wrist is
oriented appropriately. For example, when the users palm is facing down,
`waveUp` is triggered instead of `waveOut`, and `waveDown` is triggered instead
of `waveIn`. The opposite is true when the user's palm is facing up.

We also provide a class to easily detect basic patterns which we call gestures.
These gestures include clicking, double clicking, and holding a pose. You can
also use this library to debounce the pose input if you find you often trigger
poses accidentally.

Finally, the library is very flexible. Each class is templated and allows you to
mix and match parts of the library together if you only want to use specific
features.

Usage
-----

The easiest way to use this library is to create a new leaf-feature and add it
to the feature tree. For a more detailed explanation of the feature tree, see
[Explanation](#Explanation). See `ExampleCLass.h` for a simple example.

The root of the feature tree is always `RootFeature`. You can add child features
to a parent feature by passing the parent feature to the child feature in the
child feature's constructor or more conveniently the feature's factory function.

Here's a simple example of a feature tree. Try it for yourself!
```c++
RootFeature root_feature;
auto debounce = make_debounce(root_feature);
auto example = make_example(debounce);
```

Sometimes one feature, let's call it feature_a, needs access to the non-virtual
methods of another feature, let's call it feature_b. This is accomplished by
passing in feature_b to feature_a's constructor. For example, OrientationPoses
needs an Orientation feature. Note that it is not important where the features
are relative to each other in the tree.
```c++
RootFeature root_feature;
auto orientation = make_orientation(root_feature);
auto debounce = make_debounce(root_feature);
auto orientation_poses = make_orientation_poses(debounce, orientation);
auto example = make_example(orientation_poses);
```

Explanation
-----------

Coming soon...

Future Plans
------------

- More complex pattern matching, possible using machine learning.
- Ability to suppress poses if it's part of a more complicated pose.

Dependencies
------------

- [Basic-Timer](https://github.com/VoidingWarranties/Basic-Timer)
