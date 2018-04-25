#ifndef VIZ_H
#define VIZ_H


#include <memory>
#include <vector>

class Viz
{
public:
    Viz();
    void visualize();
    void setWaypoints(std::vector<double> &x, std::vector<double> &y);
    void setCarPos(double x, double y);

private:

    class impl;
    std::unique_ptr<impl> pImpl;
};

#endif // VIZ_H
