#include "fpp_controller.h"

#include <iostream>
#include <istream>
#include <ostream>

FPPController::FPPController(std::string const& ip, unsigned int output_count) :
    ControllerBase(ip,output_count)
{
}

FPPController::~FPPController() { }

bool FPPController::setTestModeOn() const {
    std::string data = "{\"mode\":\"RGBChase\",\"subMode\":\"RGBChase-RGB\",\"cycleMS\": 1000,\"colorPattern\": \"FF000000FF000000FF\",\"enabled\": 1,\"channelSet\":\"*\",\"channelSetType\":\"channelRange\"}";
    const std::string url = "/api/testmode";
    return postData(url, data, "application/json" );
}

bool FPPController::setTestModeOff() const {
    std::string data= "{\"mode\":\"RGBChase\",\"subMode\":\"RGBChase-RGB\",\"cycleMS\": 1000,\"colorPattern\": \"FF000000FF000000FF\",\"enabled\": 0,\"channelSet\":\"*\",\"channelSetType\":\"channelRange\"}";
    const std::string url = "/api/testmode";
    return postData(url, data, "application/json" );
}