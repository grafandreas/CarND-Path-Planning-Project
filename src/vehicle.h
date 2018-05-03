#ifndef VEHICLE_H
#define VEHICLE_H

#include "json.hpp"
#include "coordinates.h"
using json = nlohmann::json;

/**
 * @brief A class that holds the information about a given vehicle.
 *
 * Assumptions:
 * - All lanes are same width
 * - speed calculation can be derived from vx and vy (simply implemented)
 *
 */
class Vehicle
{

public:
    Vehicle();
    Vehicle(json &j, bool isEgo);

    double s;
    double d;
    double vx;
    double vy;
    double x;
    double y;
    double yaw; //!< yaw in degrees
    double yaw_rad; //!< yaw in radians
    int id;

    int lane; //!< Is calculated by dividing d through lane width
    double speed;  //!< Calculated from vx and vy

    XY predictPosByM(double meter);

};

#endif // VEHICLE_H
