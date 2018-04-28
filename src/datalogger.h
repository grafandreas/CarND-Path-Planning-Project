#ifndef DATALOGGER_H
#define DATALOGGER_H

#include "json.hpp"
#include <list>
using json = nlohmann::json;



class DataLogger
{
public:
    DataLogger();
    void append(json v);
    void dump();

private:
    std::list<json> l;

};

#endif // DATALOGGER_H
