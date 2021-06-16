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


**How to run**
Run ./AutosarPocApp to start all three components on the Raspberry Pi platform.

**Additional info**
Example of Message Queue taken from here:
https://www.geeksforgeeks.org/ipc-using-message-queues/


**Sync libraries needed for cross-compilation**
rsync -vR --progress -rl --delete-after --safe-links autosar@192.168.1.103:/{lib,usr,opt/vc/lib} $HOME/rpi/rootfs

**Sync solution with Raspberry**
rsync -r asbv/ autosar@192.168.1.103:~/AUTOSAR_POC/asbv

sudo i2cdetect -y 1