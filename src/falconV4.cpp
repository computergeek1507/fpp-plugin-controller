#include "falconV4.h"

#include "fpp-pch.h"
#include "common.h"
#include "settings.h"

#include <stdlib.h>
#include <cstdint>
#include <thread>
#include <cmath>

#include <iostream>
#include <istream>
#include <ostream>

FalconV4::FalconV4(std::string const& ip, unsigned int output_countm) :
    ControllerBase(ip,output_count)
{

}

FalconV4::~FalconV4() {

}

bool FalconV4::setTestModeOn( int outputs) const {
    //{"smartlife.iot.smartbulb.lightingservice":{"transition_light_state":{"ignore_default":1,"transition_period":150,"mode":"normal","hue":120,"on_off":1,"saturation":65,"color_temp":0,"brightness":10}}}
    
    const std::string cmd = "{\"smartlife.iot.smartbulb.lightingservice\":{\"transition_light_state\":{\"ignore_default\":1,\"transition_period\":" + std::to_string(period) + ",\"mode\":\"normal\",\"hue\":" 
    + std::to_string(hue) + ",\"on_off\":1,\"saturation\":" + std::to_string(saturation) + ",\"color_temp\":" + std::to_string(color_Temp) + ",\"brightness\":" + std::to_string(brightness) + "}}}";
    return sendCmd(cmd);
}

bool FalconV4::setTestModeOff() const {

    const std::string cmd = "{\"smartlife.iot.smartbulb.lightingservice\":{\"transition_light_state\":{\"ignore_default\":1,\"transition_period\":0,\"mode\":\"normal\",\"on_off\":0}}}";
    return sendCmd(cmd);
}

