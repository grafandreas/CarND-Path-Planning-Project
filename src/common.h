#ifndef COMMON_H
#define COMMON_H

#include <utility>
#include <vector>
#include "config.h"
typedef std::pair<double,double> XY;
typedef std::pair<double,double> Sd;
typedef int lane_type;

typedef std::tuple<double,double,double> XYy; //!< coordinates and yaw

inline std::vector<lane_type> neighbour_lanes(lane_type lane) {
    std::vector<lane_type> r;
    if(lane == 0)
        r = {1};
    else if(lane== 1)
        r = {0,2};
    else
        r = {1};

    return r;
}

inline double lane2d(lane_type lane) {
    auto lw = Config::getInstance()->laneWidth();
    return lw*lane+(lw/2);
}

inline bool intervalIntersect(double s1, double e1, double s2, double e2) {
    return ((s2 >= s1 && s2 <=e1) || (e2 >=s1 && e2 <= e1));
}

#endif // COMMON_H

