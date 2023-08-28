#include "genius.h"

#include <iostream>
#include <istream>
#include <ostream>

Genius::Genius(std::string const& ip, unsigned int output_count) :
    ControllerBase(ip,output_count)
{
}

Genius::~Genius() {

}

bool Genius::setTestModeOn() const {
    //{"smartlife.iot.smartbulb.lightingservice":{"transition_light_state":{"ignore_default":1,"transition_period":150,"mode":"normal","hue":120,"on_off":1,"saturation":65,"color_temp":0,"brightness":10}}}
    
    const std::string url = "/api/test_mode_enable";
    std::string data;
    bool test = getData(url, data, "application/json" );
    const std::string url2 = "/api/set_test_elements??selected_elements={elements:[\"all\"]}";
    test &= getData(url2, data, "application/json" );
    return test;
}

bool Genius::setTestModeOff() const {
    std::string data;
    const std::string url = "/api/test_mode_disable";
    return getData(url, data, "application/json" );
}