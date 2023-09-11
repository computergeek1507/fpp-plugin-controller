#include <fpp-pch.h>

#include "wled_controller.h"
#include "common.h"
#include "settings.h"

WLEDController::WLEDController(std::string const& ip) :
    ControllerBase(ip)
{
}

WLEDController::~WLEDController() { }

bool WLEDController::setTestModeOn() const {

    std::string getdata;
    const std::string geturl = "/json/state";
    getData(geturl, getdata, "application/json" );
    Json::Value v = LoadJsonFromString(getdata);
    
    std::string data = "{\"seg\":[";
    for (int i = 0; i < v["seg"].size(); i++) {
        data += "{\"fx\":34},";
    }
    data.pop_back();//remove last comma
    data += "]}";

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