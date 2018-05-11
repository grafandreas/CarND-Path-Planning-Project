#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <experimental/optional>

class Config
{


public:
    static Config * getInstance() {
        if(!_instance)
            _instance = new Config ();

        return _instance;
    }


    int w();
    int h();
    double VizTranslateX();
    double VizTranslateY();
    double VizScale();
    double laneWidth();
    int numLanes();
    double sensorRangeFront();
    double sensorRangeBack();
    double targetSpeed();
    double speedLimit();
    double planAhead();

    double trajectoryWaypointDist();
    double trajectoryTrajectoryLength();
    double trajectoryTrajectoryMin();
    double trajectoryReuseNPoints();

    double speedIncrease();
    std::experimental::optional<double> initialS();



private:
    static Config *_instance;
    Config();

    class impl;
    std::unique_ptr<impl> pImpl;
};


#endif // CONFIG_H
