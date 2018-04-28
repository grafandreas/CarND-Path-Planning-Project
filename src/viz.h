#ifndef VIZ_H
#define VIZ_H


#include <memory>
#include <vector>
#include "vehicle.h"

class Viz
{
public:
    Viz();
    void visualize();
    void setWaypoints(std::vector<double> &x, std::vector<double> &y);
    void setCarPos(double x, double y, double yaw, double speed);
    void setWaypoint(double x, double y);
    void setVehicles(std::vector<Vehicle> & vehicles);

private:

    class impl;
    std::unique_ptr<impl> pImpl;
};

#endif // VIZ_H
