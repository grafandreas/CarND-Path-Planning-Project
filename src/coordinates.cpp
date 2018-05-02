#include "coordinates.h"

#include <algorithm>

XY rotate(XY c, double theta) {
    return XY(c.first*cos(theta)-c.second*sin(theta),
              c.first*sin(theta)+c.second*cos(theta));
}

XY world2car(XY carp, XY arg, double yaw ) {
    auto r = XY(arg.first-carp.first,arg.second-carp.second);
    return rotate(r,-yaw);
}

XY car2world(XY carp, XY arg, double yaw ) {
//      auto r = XY(arg.first+carp.first,arg.second+carp.second);
//      return rotate(r,yaw);

    auto rot = rotate(arg,yaw);
    return XY(rot.first+carp.first,rot.second+carp.second);
}

void world2car(XY carp,std::vector<XY> & in, std::vector<XY> & out, double yaw) {
    std::transform(in.begin(),in.end(),std::back_inserter(out),
                   [carp,yaw](XY& arg){return world2car(carp,arg,yaw);});
}

void car2world(XY carp,std::vector<XY> & in, std::vector<XY> & out, double yaw) {
    std::transform(in.begin(),in.end(),std::back_inserter(out),
                   [carp,yaw](XY& arg){return car2world(carp,arg,yaw);});
}

/**
 * @brief split
 *          Convert a XY list into two single lists
 *
 * @param in
 * @param xl
 * @param yl
 */
void split  (std::vector<XY> & in, std::vector<double> & xl, std::vector<double> & yl){
    for(auto const &xy:in) {
        xl.push_back(xy.first);
        yl.push_back(xy.second);
    }
}
