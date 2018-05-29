#ifndef VEHICLE_H
#define VEHICLE_H

#include "json.hpp"
#include "coordinates.h"
#include <vector>

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
    double collision_time (const Vehicle &other) const  ;
    std::pair<double,double> collision_range (const Vehicle &other) const  ;
    void fillNextTickPositions( std::vector<double> & x,  std::vector<double> & y, const int count);

};

inline double coll_time(double s0, double v0, double s1, double v1) {
    auto t = (s0-s1)/(v1-v0);
    return t;
}

// |s0+v0t-s1-v1t| <= car_len
// -car_len <= s0+v0-s1-v1 <= car_len
// d = s0-s1
// -car_len <= d+(v0-v1)t <= car_len
// -car_len-d >= (v0-v1)t >= car_len -d
//
inline std::pair<double,double> coll_range(double s0, double v0, double s1, double v1, double car_len) {
    auto d = s0-s1;

    auto l = (-car_len-d)/(v0-v1);
    auto r = (car_len-d)/(v0-v1);
    return std::pair<double,double> (std::min(l,r),std::max(l,r));

}
#endif // VEHICLE_H
