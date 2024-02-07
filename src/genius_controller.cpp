#include <fpp-pch.h>

#include "genius_controller.h"
#include "common.h"
#include "settings.h"

GeniusController::GeniusController(std::string const& ip) :
    ControllerBase(ip)
{
}

GeniusController::~GeniusController() {

}

bool GeniusController::setTestModeOn() const {
    std::string data;
    const std::string url = "/api/test_mode_enable";
    bool test = getData(url, data, "application/json" );
    const std::string url2 = "/api/set_test_elements?selected_elements={elements:[\"all\"]}";
    test &= getData(url2, data, "application/json" );
    return test;
}
bool GeniusController::setTestModeOff() const {
    std::string data;
    const std::string url = "/api/test_mode_disable";
    return getData(url, data, "application/json" );
}

bool GeniusController::setTestModeOnPort(int portNum) const {
    std::string data;
    const std::string url = "/api/test_mode_enable";
    bool test = getData(url, data, "application/json" );
    //["o","0"]
    //{elements:[["o","0"]]}
    const std::string url2 = "/api/set_test_elements?selected_elements={elements:[[\"o\",\"" + std::to_string(portNum - 1) + "\"]]}";
    test &= getData(url2, data, "application/json" );
    return test;
}

bool GeniusController::isInTestMode() const
 {
    // /api/state
    //"test_mode_enabled": true,

    std::string data;
    const std::string url = "/api/state";
    bool work = getData(url, data, "application/json" );
    Json::Value v = LoadJsonFromString(data);

    if (v["system"]["test_mode_enabled"].asBool()) {
        return true;
    }

    return false;
 }