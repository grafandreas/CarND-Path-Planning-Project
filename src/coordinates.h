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


#endif // COORDINATES_H
