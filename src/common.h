#ifndef COMMON_H
#define COMMON_H

#include <utility>
#include <vector>
#include <iostream>
#include <cmath>
#include <limits>

#include "config.h"
typedef std::pair<double,double> XY;
typedef std::pair<double,double> Sd;
typedef int lane_type;

typedef std::tuple<double,double,double> XYy; //!< coordinates and yaw

/*
 * The duration of a tick in caclulation in the simulator
 *
 */

#define TICK 0.02
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

inline void dump(std::string prefix, const std::vector<XY> & in, int max_els) {
    std::cout << prefix ;
    for(int i = 0; i < (in.size()<max_els?in.size():max_els);i++) {
        std::cout << in.at(i).first <<"," << in.at(i).second <<"  ";
    }
    std::cout << std::endl;
}

inline void dump(std::string prefix, const std::vector<double> & x,  const std::vector<double> &y,int max_els) {
    std::cout << prefix ;
    for(int i = 0; i < (x.size()<max_els?x.size():max_els);i++) {
        std::cout << x.at(i) <<"," << y.at(i) <<"  ";
    }
    std::cout << std::endl;
}

template<typename T>
static bool AreEqual(T f1, T f2) {
  return (std::fabs(f1 - f2) <= std::numeric_limits<T>::epsilon() * std::fmax(fabs(f1), fabs(f2)));
}


#endif // COMMON_H

