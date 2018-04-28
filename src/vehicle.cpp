#include "vehicle.h"
#include "config.h"



Vehicle::Vehicle()
{

}

Vehicle::Vehicle(json j)
{
//    id = j["id"];
//    s = j["s"];
//    d = j["d"];
//    x = j["x"];
//    y = j["y"];
//    vx = j["vx"];
//    vy = j["vy"];

    id = j[0];
    x = j[1];
    y = j[2];
    vx = j[3];
    vy = j[4];
    s = j[5];
    d = j[6];

    lane = (int)(d/Config::getInstance()->laneWidth());

}
