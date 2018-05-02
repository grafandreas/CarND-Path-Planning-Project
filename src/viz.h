#ifndef VIZ_H
#define VIZ_H


#include <memory>
#include <vector>
#include "vehicle.h"

/**
 * @brief A helper class to visualize some of the values of the simulation
 *
 */
class Viz
{
public:
    Viz();
    void visualize();
    void setWaypoints(std::vector<double> &x, std::vector<double> &y);
    void setCarPos(double x, double y, double yaw, double speed);
    void setWaypoint(double x, double y);
    void setVehicles(std::vector<Vehicle> & vehicles);
    void setLaneSpeeds(std::vector<double> & speeds);
    void setPrevPath(const std::vector<double> &x, const std::vector<double> &y);

private:

    class impl;
    struct pimpl_deleter { void operator()(impl*) const; };
    std::unique_ptr<impl,pimpl_deleter> pImpl;


};

#endif // VIZ_H
