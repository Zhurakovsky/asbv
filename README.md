# Self-Balancing vehicle robotics motorcycle POC

**Description:**
Usual behavior of a motorcycle is falling aside if current linear speed is not enough to keep balancing.
The self-balancing effect can be achieved by using quite heavy and expensive HW gyroscope. 
But nowadays, thanks to new generation of the Inertial Measurement Units, that provides high-frequency
gyroscope and acceleration data, self-balancing effect can be achieved by steer-by-wire and indirect steering.  

IMU data can be processed by PID computation unit to obtain the most effective angle of handlebar rotation.

**The purpose of the proposed POC:**

to create AUTOSAR solution, consist of three Software components (SWC):

Sensor SWC, Controller SWC, Actuators SCW that works together to establish self-balancing of the motorcycle target.

**Prerequirements**
1. Ensure that cross-compilation available on your host computer: https://github.com/Pro/raspi-toolchain


