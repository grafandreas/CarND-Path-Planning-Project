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

double Sensor::laneSpeed(vector<Vehicle> vehicles, int lane, int s) {
    auto vehic = closestVehicle(vehicles,lane,s);
    if(vehic) {
        cout << "--> " <<lane << " " << (*vehic).id << " " << s << " " << (*vehic).s  <<" " << (*vehic).speed << endl;
    } else {
        cout << "--> "<<lane<< "/" << endl;
    }
    if(vehic)
        return (*vehic).speed;
    else
        return 100.0;
}

vector<double> Sensor::laneSpeeds(vector<Vehicle> vehicles, int s) {
    vector<double> r;
    for(int i =  0; i <Config::getInstance()->numLanes();i++)
        r.push_back(laneSpeed(vehicles,i,s));

    cout << "S: " << r.at(0) << " " << r.at(1) <<" " << r.at(2) << endl;
    return r;
}

lane_type Sensor::fastestLaneFrom(vector<Vehicle> vehicles, int s, lane_type currentLane) {
    auto speeds = laneSpeeds(vehicle,s);
    auto fastest = std::max_element(speeds.begin(),speeds.end());
    if(*fastest == speeds.at(currentLane))
        return currentLane;

    return fastest-speeds.begin();
}
