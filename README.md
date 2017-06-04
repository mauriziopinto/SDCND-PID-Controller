# SDNCD-PID-Controller
Self-Driving Car Engineer Nanodegree Program

## Summary

This repository contains a C++ implementation of a PID controller to be used with the Udacity simulator available at https://github.com/udacity/self-driving-car-sim. It has been implemented as part of the Udacity Self-Driving Car Engineer Nanodegree Program.

## PID tuning

The tuning of the Kp, Ki, and Kd gains has been implemented manually. First the Kp value has been tuned until the car oscillated more or less regularly, then the Kd value has been tuned until the car managed to successfully complete the whole track, then the Ki has been tuned to reach acceptable stability (i.e. a local minimum)

Please read here below the sequence of steps:

* std::vector<double> p = {0.2,0.0,0.00}; // iteration 1 -> diverges (Kp only)
* std::vector<double> p = {1.2,0.0,0.00}; // iteration 2 -> diverges (Kp only)
* std::vector<double> p = {0.5,0.0,0.00}; // iteration 3 -> diverges (Kp only)
* std::vector<double> p = {0.250,0.0,0.00}; // iteration 4 -> slightly diverges (Kp only)
* std::vector<double> p = {0.225,0.0,0.00}; // iteration 5 -> quite stable oscillations (Kp only)
* std::vector<double> p = {0.225,0.0,1.00}; // iteration 6 -> able to complete the track, some oscillations on the bridge part (Kp, Kd only)
* std::vector<double> p = {0.225,0.0,2.00}; // iteration 7 -> able to complete the track, less oscillations on the bridge part  (Kp, Kd only) -> candidate 1
* std::vector<double> p = {0.225,0.0,3.00}; // iteration 8 -> able to complete the track, more oscillations during curves (Kp, Kd only)
* std::vector<double> p = {0.225,0.0,2.50}; // iteration 9 -> able to complete the track, still more oscillations during curves (Kp, Kd only)
* std::vector<double> p = {0.225,0.0,2.25}; // iteration 10 -> able to complete the track, still more oscillations during curves (Kp, Kd only) -> candidate 2
* std::vector<double> p = {0.225,1.0,2.25}; // iteration 11 -> goes off-track almost immediately (Kp, Kd, Ki)
* std::vector<double> p = {0.225,0.1,2.25}; // iteration 12 -> goes off-track almost immediately (Kp, Kd, Ki)
* std::vector<double> p = {0.225,0.001,2.25}; // iteration 13 -> able to complete the track, some minor oscillations during curves (Kp, Kd only) -> candidate 3
* std::vector<double> p = {0.275,0.001,2.55}; // iteration 14 -> fine tuning, average CTE printed out during execution -> candidate 4
* std::vector<double> p = {0.250,0.001,2.75}; // iteration 15 -> fine tuning, average CTE printed out during execution -> candidate 5
* std::vector<double> p = {0.235,0.001,2.75}; // iteration 16 -> fine tuning, average CTE printed out during execution -> candidate 6
* std::vector<double> p = {0.235,0.005,2.75}; // iteration 17 -> steering during curves is less abrupt -> candidate 7 -> selected
* std::vector<double> p = {0.235,0.008,2.75}; // iteration 18 -> minor oscillations
* std::vector<double> p = {0.235,0.003,2.75}; // iteration 19 -> some oscillations, especially when leaving the bridge

The following values have been selected (iteration 17):

* *Kp: 0.235*
* *Ki: 0.005*
* *Kd: 2.75*

The values above allow the car to successfully complete the track and minimize the oscillations.

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets) == 0.13, but the master branch will probably work just fine
  * Follow the instructions in the [uWebSockets README](https://github.com/uWebSockets/uWebSockets/blob/master/README.md) to get setup for your platform. You can download the zip of the appropriate version from the [releases page](https://github.com/uWebSockets/uWebSockets/releases). Here's a link to the [v0.13 zip](https://github.com/uWebSockets/uWebSockets/archive/v0.13.0.zip).
  * If you run OSX and have homebrew installed you can just run the ./install-mac.sh script to install this
* Simulator. You can download these from the [project intro page](https://github.com/udacity/CarND-PID-Control-Project/releases) in the classroom.

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 