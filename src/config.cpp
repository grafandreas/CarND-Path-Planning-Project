#include "config.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

class Config::impl {
public:
    json j;

};


Config::Config() : pImpl(new impl)
{
    std::ifstream ifs("config.json");
    pImpl->j = json::parse(ifs);

}

int Config::w() {
    return pImpl->j["viz"]["w"];
}

int Config::h() {
    return pImpl->j["viz"]["h"];
}

double Config::VizTranslateX() {
    return pImpl->j["viz"]["translate"]["x"];
}

double Config::VizTranslateY() {
    return pImpl->j["viz"]["translate"]["y"];
}

double Config::VizScale() {
    return pImpl->j["viz"]["scale"];
}

double Config::laneWidth() {
    return pImpl->j["laneWidth"];
}

double Config::sensorRangeFront() {
    return pImpl->j["sensor"]["range-front"];
}

double Config::sensorRangeBack() {
    return pImpl->j["sensor"]["range-back"];
}

int Config::numLanes() {
    return pImpl->j["numLanes"];
}

double Config::targetSpeed() {
    return pImpl->j["targetSpeed"];
}

double Config::planAhead() {
    return pImpl->j["planAhead"];
}

double Config::trajectoryWaypointDist() {
    return pImpl->j["trajectory"]["waypoint-dist"];
}

double Config::trajectoryTrajectoryLength(){
    return pImpl->j["trajectory"]["trajectory-length"];
}

double Config::trajectoryTrajectoryMin(){
    return pImpl->j["trajectory"]["trajectory-min"];
}


std::experimental::optional<double> Config::initialS() {
    if(pImpl->j["debug"]["initial-s"].is_null())
        return {};
    else
        return ((double)(pImpl->j["debug"]["initial-s"]));
}


Config* Config::_instance = 0;
