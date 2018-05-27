# CarND-Path-Planning-Project
Self-Driving Car Engineer Nanodegree Program


### Goals
In this project your goal is to safely navigate around a virtual highway with other traffic that is driving +-10 MPH of the 50 MPH speed limit. You will be provided the car's localization and sensor fusion data, there is also a sparse map list of waypoints around the highway. The car should try to go as close as possible to the 50 MPH speed limit, which means passing slower traffic when possible, note that other cars will try to change lanes too. The car should avoid hitting other cars at all cost as well as driving inside of the marked road lanes at all times, unless going from one lane to another. The car should be able to make one complete loop around the 6946m highway. Since the car is trying to go 50 MPH, it should take a little over 5 minutes to complete 1 loop. Also the car should not experience total acceleration over 10 m/s^2 and jerk that is greater than 10 m/s^3.


## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Cd to ../run
4. Run it: `../build/path_planning`.


# Code structure

The structure of the code has been considerably changed to be more modular

## Configuration file

Configuration of many parameters is not done through compiled variables, but through a jason configuration file. The configuration file is stored in run/ directory. That's why the simulator has to be run from that directory, since the current working directory
is being used to look for that file.

```json
{
    "viz" : {
        "w" : 1300,
        "h" : 1000,
        "translate" : {
            "x" : 0.0,
            "y" : -1000.0
        },
        "scale" : 0.45
    },
    "x_debug" : {
        "initial-s" : 2400.0
    },

    "laneWidth" : 4.0,
    "numLanes" : 3,
    "targetSpeed" : 21.01,
    "speedLimit" : 22.352,
    "speedIncrease" : 0.08,
    "speedTolerance" : 0.2,
    "planAhead" : 50.0,
    "sensor" : {
        "range-front" : 100.0,
        "range-back" : 30.0
    },
    "trajectory" : {
        "waypoint-dist" : 30.0,
        "trajectory-length" : 120.0,
        "trajectory-min" : 30.0,
        "reuse-n-points" : 13
    }
}
```

## Code structure

A number of classes has been introduced to make the code more modular:

- vehicle.cpp : Information about vehicles, no matter if ego or other vehicles
- sensor.cpp : Information about the vehicles that can be seen from the car
- trajectory.cpp : Caclulation of the trajectory

## Path plannning

The path planning is executed in the following steps:

1. Find the fastest / best lane to drive in
2. If best lane is not the current lane, see if lane can be changed safely (no collisions)
3. If (2) results in a lane change, plan new trajectory, otherwise continue on current lane.

### Lane driving / speed determination

To determine the best lane, ```Sensor::fastestLaneFrom(vector<Vehicle> vehicles, int s, lane_type currentLane)``` is invoked.
