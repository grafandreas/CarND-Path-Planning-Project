#ifndef MAPSERVICE_H
#define MAPSERVICE_H


#include <memory>
#include <vector>

class MapService
{
public:
    MapService(std::vector<double> & wp_x, std::vector<double> & wp_y);


private:

    class impl;
    std::unique_ptr<impl> pImpl;


};

#endif // MAPSERVICE_H
