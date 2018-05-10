#ifndef COORDINATES_H
#define COORDINATES_H

#include "common.h"
#include <vector>
#include <math.h>

XY world2car(XY carp, XY arg, double yaw = 0.0);
XY car2world(XY carp, XY arg, double yaw = 0.0);

void world2car(XY carp,std::vector<XY> & in, std::vector<XY> & out, double yaw = 0.0);
void car2world(XY carp,std::vector<XY> & in, std::vector<XY> & out, double yaw = 0.0);

void split  (std::vector<XY> & in, std::vector<double> & xl, std::vector<double> & yl);
XY rotate(XY c, double theta) ;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
inline double deg2rad(double x) { return (x * M_PI) / 180; }
inline double rad2deg(double x) { return (x * 180) / M_PI; }
inline double distance(double x1, double y1, double x2, double y2)
{
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}


/**
 * @brief length - Helper to calculate the distance a long a list of points.
 * @param x
 * @param y
 * @param idx
 */
double calc_length(const std::vector<double> &x, const std::vector<double> &y, const int idx) ;

/**
 * @brief calc_speed calculate the speed between to points, assuming that the distance is covered by a Tick
 * @param x0
 * @param y0
 * @param x1
 * @param y1
 * @return speed in m/s
 */


inline double calc_speed(const double x0, const double y0, const double x1, const double y1) {
    auto dist = distance(x0,y0,x1,y1);
    auto res = dist * (1/TICK);
    return res;
}

/**
 * @brief dist_per_tick distance for a given speed, per tick
 * @param speed in m/s
 * @return
 */
inline double dist_per_tick(const double speed) {
    return speed/TICK;
}

#endif // COORDINATES_H
