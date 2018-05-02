#include "trajectory.h"

#include "spline.h"
#include <algorithm>
#include <limits>
#include <iostream>

// TODO: Make sure that waypoints are in car coordinates;

class Trajectory::impl {
public:
    std::vector<XY> &points;
    int resolution;
    tk::spline s;
    double max_x = 0.0, min_x = std::numeric_limits<double>::max();

    impl(std::vector<XY> & p) : points(p), resolution(p.size()) {

        std::vector<double> x,y;

        std::transform(p.begin(),p.end(),std::back_inserter(x),
                       [](const XY& it) {return it.first;});
        std::transform(p.begin(),p.end(),std::back_inserter(y),
                       [](const XY& it) {return it.second;});
        max_x = *std::max_element(x.begin(),x.end());
        min_x = *std::min_element(x.begin(),x.end());

        std::cout<<p.size()<<std::endl;
        std::cout<<x.size()<<std::endl;
        for(auto xp: x) {
            std::cout << xp << std::endl;
        }

        s.set_points(x,y);
    }

    impl(std::vector<XY> & p, int r) : points(p), resolution(r) {
        std::vector<double> x,y;

        std::transform(p.begin(),p.end(),std::back_inserter(x),
                       [](const XY& it) {return it.first;});
        std::transform(p.begin(),p.end(),std::back_inserter(y),
                       [](const XY& it) {return it.second;});
        max_x = *std::max_element(x.begin(),x.end());
        min_x = *std::min_element(x.begin(),x.end());

        std::cout<<p.size()<<std::endl;
        std::cout<<x.size()<<std::endl;
        for(auto xp: x) {
            std::cout << xp << std::endl;
        }


        s.set_points(x,y);
    }

    void fillLists(std::vector<double> & xl, std::vector<double> & yl);
};
void Trajectory::pimpl_deleter::operator()(Trajectory::impl*ptr) const { delete ptr; }

Trajectory::Trajectory(std::vector<XY> & points) : pImpl(new impl(points))
{

}

Trajectory::Trajectory(std::vector<XY> & points, int r) : pImpl( new impl(points,r))
{

}

//void Trajectory::fillLists(std::vector<double> &xl, std::vector<double> &yl) {
//    std::transform(pImpl->points.begin(), pImpl->points.end(), std::back_inserter(xl),
//                   [](XY p){return p.first;});

//    std::transform(pImpl->points.begin(), pImpl->points.end(), std::back_inserter(yl),
//                   [](XY p){return p.second;});
//}

void Trajectory::fillLists(std::vector<double> &xl, std::vector<double> &yl) {
    for(int i = 0; i<pImpl->resolution;i++) {
        auto xy = (*this)(i);
        xl.push_back(xy.first);
        yl.push_back(xy.second);
    }
}


XY Trajectory::operator()(int pos) {
    auto delta = pImpl->max_x-pImpl->min_x;
    auto stepSize = delta/pImpl->resolution;

    auto x = stepSize*pos;
    return XY(pImpl->min_x+x,pImpl->s(pImpl->min_x+x));
}
