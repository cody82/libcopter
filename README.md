# libcopter, or rather libSG500 for now

This aims to be a library that supports various similar cheap
chinese copters that are controlled via WiFi and have a camera on
board. Currently, the **SG500** and the **JJRC blue crab** drones
are supported.

The library is written in **C++**, and **Python 3** bindings are
available.

The goal of this library is to support controlling the drone
as well as retrieving telemetry information and the video stream
for further use in the OpenCV library.

## Example

For a [C++ example, see here](examples/cpp_demo).

Below is an untested python example:

```
#!/usr/bin/python3

import libcopter
import cv2 # opencv

copter = libcopter.SG500()
copter.initialize()

copter.takeoff()
while True:
	copter.command(0.5,0,0,0)  # go right
	imgs, _ = copter.poll_data()
	if len(imgs) > 0:
		cv2.imshow("img", imgs[-1].frame)
copter.land()
```


## Current state

Currently, I'm still reverse engineering the drone. Most I've
already figured out, but there is still some way to go. Find
my reverse engineering attempts [here](reverse_engineering).

There's [a python script](dronecontrol) that can generate
steering packets and can successfully take-off and land
the copter.

Also, there's [a video stream parser](parse_tcpstream) written
in C++ that splits the stream into telemetry data and a video
stream and parses both. I am currently working on python bindings.

**This project is in a _very_ early state, do not expect much
from it yet.**

## Troubleshooting

**My (new) drone keeps flying upwards until it hits the ceiling
and is uncontrollable.** This is not a libcopter problem. You
have to calibrate your drone first, e.g. by following these steps:

  1. Connect your smartphone to the drone
  2. Launch the app
  3. Enable the control and the altitude hold modes using the buttons
     on the top, so that the control sticks and the takeoff/land/panic
     buttons appear.
  4. Press the options button, and select the gyro button in the menu
     that opens. Your drone should flash.

Calibration support will soon be available in libcopter. The calibration
data seems to persist over drone reboots.
