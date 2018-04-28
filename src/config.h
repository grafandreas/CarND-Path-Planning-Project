#ifndef CONFIG_H
#define CONFIG_H

#include <memory>

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
    double sensorRangeFront();
    double sensorRangeBack();

private:
    static Config *_instance;
    Config();

    class impl;
    std::unique_ptr<impl> pImpl;
};


#endif // CONFIG_H
