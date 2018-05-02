#ifndef COORDINATES_H
#define COORDINATES_H

#include "common.h"
#include <vector>

XY world2car(XY carp, XY arg, double yaw = 0.0);
XY car2world(XY carp, XY arg, double yaw = 0.0);

void world2car(XY carp,std::vector<XY> & in, std::vector<XY> & out, double yaw = 0.0);
void car2world(XY carp,std::vector<XY> & in, std::vector<XY> & out, double yaw = 0.0);


#endif // COORDINATES_H
