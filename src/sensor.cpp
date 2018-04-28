#include "sensor.h"
#include "algorithm"
#include "config.h"
Sensor::Sensor(SensorType t, MapService & service) : type(t),map_service(&service)
{

}

bool compare_Vehicle_s(const Vehicle & x, const Vehicle& y)
{
    return x.s < y.s;
}


std::experimental::optional<Vehicle> Sensor::closestVehicle(vector<Vehicle> vehicles, int lane, int s) {
    vector<Vehicle> same_lane;

    double range = Config::getInstance()->sensorRangeFront();
    if(type == BACK)
        range = Config::getInstance()->sensorRangeFront();

    // TODO: Handle wrap around at end of track
    std::copy_if(vehicles.begin(), vehicles.end(), std::back_inserter(same_lane),
                 [lane,s,range,this](const Vehicle &it) {

                    if(it.lane != lane)
                        return false;

                    return true;

                    auto dist = it.s - s;
                    if(type == FRONT && (dist >=0 && dist<= range) )
                        return true;

                    if(type == BACK && (dist <=0 && dist>= -range) )
                         return true;

                    return false;

    });

    if(same_lane.size() == 0)
            return {};

    if(type == FRONT)
        return * std::min_element(same_lane.begin(), same_lane.end(), compare_Vehicle_s);
    else
        return * std::max_element(same_lane.begin(), same_lane.end(), compare_Vehicle_s);

}
