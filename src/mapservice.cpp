#include "mapservice.h"

class MapService::impl {
public:
    std::vector<double> & wp_x;
    std::vector<double> & wp_y;
    int no_wp;

    impl(std::vector<double> & x, std::vector<double> & y) : wp_x(x), wp_y(y) {
        no_wp = wp_x.size();
    }
};

MapService::MapService(std::vector<double> & wp_x, std::vector<double> & wp_y) :  pImpl(new impl(wp_x,wp_y))
{

}

