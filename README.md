# DeltaT

UnitGenerator for [SuperCollider](https://github.com/supercollider/supercollider) tracking time difference of onsets between two signals. Originally used to locate hits on a wooden board using piezo microphones.

## Building

```
mkdir build
cd build
cmake -DSC_PATH=/path/to/sc3source/ ..
cmake --build .
```

## Installing

Copy `DeltaT.scx` or `DeltaT.so` and `DeltaT.sc` into your Extensions folder. You can find out which one that is by evaluating `Platform.userExtensionDir` in sclang. You can install the UGen system-wide by copying to `Platform.systemExtensionDir`.
