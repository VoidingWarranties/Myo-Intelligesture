Myo Intelligesture
==================

A library for Myo which allows more complex gestures
----------------------------------------------------

For example the library could enable you to sense when the user performs the thumb-to-pinky gesture twice in a row (i.e. double clicking) which could perform a different action than a single thumb-to-pinky gesture. The sensitivity of this could be controlled similar to how OS X allows you to change the sensitivity of a mouse double click.

The library could also allow you to sense when the user performs a continued action (holding a pose) vs a quick action.

Maybe the library could take advantage of machine learning to easily learn new pose combinations.

The library should also provide the ability to debounce the gestures. For example if a user performs a pose for only a millisecond, it was probably a mistake. Equally, if a user is performing a pose and then switches to the rest pose for a millisecond before performing the same pose again, it was also probably a mistake. This could be easily implemented by simply ignoring short bursts of poses (i.e. treat the rest post the same as all of the other poses).

The library would allow you to provide a callback function for when an action supported by the library is detected.

The library will support pattern recognition of poses too (i.e. have a specific pattern of short / long thumb-to-pinky poses which unlocks the device).

Implement a short timeout duration after an accepted pose to prevent unwanted pose recognitions. This will specifically help when opening your hand after the fist pose as it will sometimes be incorrectly recognized as a finger spread pose.

Dependencies
------------
[Boost](http://www.boost.org/)
