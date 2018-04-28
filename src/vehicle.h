#ifndef VEHICLE_H
#define VEHICLE_H

#include "json.hpp"

using json = nlohmann::json;

class Vehicle
{

public:
    Vehicle();
    Vehicle(json);

    double s;
    double d;
    double vx;
    double vy;
    double x;
    double y;
    int id;

    int lane;
    double speed;



};

#endif // VEHICLE_H
