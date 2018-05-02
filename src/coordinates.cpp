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
      auto r = XY(arg.first+carp.first,arg.second+carp.second);
      return rotate(r,yaw);
}

void world2car(XY carp,std::vector<XY> & in, std::vector<XY> & out, double yaw) {
    std::transform(in.begin(),in.end(),std::back_inserter(out),
                   [carp,yaw](XY& arg){return world2car(carp,arg,yaw);});
}

void car2world(XY carp,std::vector<XY> & in, std::vector<XY> & out, double yaw) {
    std::transform(in.begin(),in.end(),std::back_inserter(out),
                   [carp,yaw](XY& arg){return car2world(carp,arg,yaw);});
}
