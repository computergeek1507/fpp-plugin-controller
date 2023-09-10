#include <fpp-pch.h>

#include "wled_controller.h"
#include "common.h"
#include "settings.h"

WLEDController::WLEDController(std::string const& ip, unsigned int output_count) :
    ControllerBase(ip,output_count)
{
}

WLEDController::~WLEDController() { }

bool WLEDController::setTestModeOn() const {
    const std::string data = "{\"seg\":[{\"fx\":34}]}";
    const std::string url = "/json";
    bool test = postData(url, data, "application/json" );
    const std::string data2 = "{\"on\":true}";
    test &= postData(url, data2, "application/json" );
    return test;
}

bool WLEDController::setTestModeOff() const {
    const std::string data = "{\"on\":false}";
    const std::string url = "/json";
    return postData(url, data, "application/json" );
}

bool WLEDController::isInTestMode() const {
    // /json/state
    std::string data;
    const std::string url = "/json/state";
    getData(url, data, "application/json" );
    Json::Value v = LoadJsonFromString(data);
    if (v["on"].asBool()) {
        return true;
    }
    return false;
}