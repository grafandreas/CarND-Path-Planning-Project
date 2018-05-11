#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <memory>
#include <vector>
#include "common.h"

/** A class for calculating and working with
* trajectories.
*/

class Trajectory
{

public:
    Trajectory(std::vector<XY> & points);
    Trajectory(std::vector<XY> & points, int resolution);

    XY operator()(int pos) ;


    // Split the list of XY points into two points of the
    // given dimensions (x,y). So that it can be used e.g.
    // for sending to the simulator.
    void fillLists(std::vector<double> & xl, std::vector<double> & yl);

    void fillLists(std::vector<XY> &out);

    void fillLists(std::vector<double> & xl, std::vector<double> & yl, double initialSpeed, double targetSpeed, double startX);

    void fillLists(std::vector<XY> & out, double initialSpeed, double targetSpeed, double startX);

private:

    class impl;
    struct pimpl_deleter { void operator()(impl*) const; };
    std::unique_ptr<impl,pimpl_deleter> pImpl;

};

#endif // TRAJECTORY_H
