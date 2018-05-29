#include "vehicle.h"
#include "config.h"

#include "coordinates.h"
#include "math.h"


Vehicle::Vehicle()
{

}


Vehicle::Vehicle(json & j, bool isEgo)
{
//    id = j["id"];
//    s = j["s"];
//    d = j["d"];
//    x = j["x"];
//    y = j["y"];
//    vx = j["vx"];
//    vy = j["vy"];
    if(isEgo) {
        x = j[1]["x"];
        y = j[1]["y"];
        s = j[1]["s"];
        d = j[1]["d"];
        yaw = j[1]["yaw"];
        yaw_rad = deg2rad(yaw);

        speed = j[1]["speed"];
        speed *= 0.44704; // Convert mph to m_per_s

    } else {
        id = j[0];
        x = j[1];
        y = j[2];
        vx = j[3];
        vy = j[4];
        s = j[5];
        d = j[6];

        lane = (int)(d/Config::getInstance()->laneWidth());
        // TODO: Calculate yaw for non-ego cars (not necessarily required).

        yaw = 0.0;
        yaw_rad = 0.0;

        // TODO: deal with cases where vehicles does not follow s, but follows d
        //
        speed = sqrt(vx*vx+vy*vy);
    }

    lane = (int)(d/Config::getInstance()->laneWidth());
}

XY Vehicle::predictPosByM(double meter) {
    auto xoffs = cos(yaw_rad) *meter;
    auto yoffs = sin(yaw_rad) * meter;
    return XY(x+xoffs,y+yoffs);
}

void Vehicle::fillNextTickPositions( std::vector<double> & x, std::vector<double> & y, const int count) {
    auto mspeed = (speed >0.0) ? speed :  Config::getInstance()->speedIncrease();
    for(int i = 0; i < count; i++) {
        auto inc = dist_per_tick(mspeed);
        auto p = predictPosByM(inc);
        x.push_back(p.first);
        y.push_back(p.second);
    }
}

double Vehicle::collision_time (const Vehicle &other) const {
    return coll_time(s,speed,other.s,other.speed);
}

std::pair<double,double> Vehicle::collision_range (const Vehicle &other) const  {
    return coll_range(s,speed,other.s,other.speed,Config::getInstance()->collisionBuffer());
}
