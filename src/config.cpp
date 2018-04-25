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



Config* Config::_instance = 0;
