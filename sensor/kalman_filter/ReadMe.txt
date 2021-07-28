To work with Kalman Filter you need "Eigen" library.
Install: https://roboticslab-uc3m.github.io/installation-guides/install-eigen.html
Settings for MVS: http://eigen.tuxfamily.org/index.php?title=IDEs#Visual_Studio
Getting started: http://eigen.tuxfamily.org/dox/GettingStarted.html
Quick reference guide: http://eigen.tuxfamily.org/dox/group__QuickRefPage.html


To fix the error "fatal error: Eigen/Dense: No such file or directory compilation terminated." please take the following steps:

1) cd /usr/local/include (Sometimes eigen3 is located under /usr/include/)
2) sudo ln -sf eigen3/Eigen Eigen

You should now be able to include the header by:  #include <Eigen/Dense>