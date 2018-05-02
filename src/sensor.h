#ifndef SENSOR_H
#define SENSOR_H

#include "mapservice.h"
#include <experimental/optional>
#include "vehicle.h"
#include <vector>

using namespace std;

enum SensorType {
    FRONT,
    BACK
};

/**
 * @brief The Sensor class
 */
class Sensor
{
public:
    Sensor(SensorType t, MapService & service);
    std::experimental::optional<Vehicle> closestVehicle(vector<Vehicle> vehicles, int lane, int s);
    double laneSpeed(vector<Vehicle> vehicles, int lane, int s);
    vector<double> laneSpeeds(vector<Vehicle> vehicles, int s);

protected:
    SensorType type;
    MapService * map_service;
};

#endif // SENSOR_H
