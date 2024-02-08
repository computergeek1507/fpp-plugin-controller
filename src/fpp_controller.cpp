#include <fpp-pch.h>

#include "fpp_controller.h"
#include "common.h"
#include "settings.h"

FPPController::FPPController(std::string const& ip) :
    ControllerBase(ip)
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

bool FPPController::setTestModeOnPort(int portNum) const {

    auto [ start, end ] = getPortChannels(portNum);
    std::string data= "{\"mode\":\"RGBChase\",\"subMode\":\"RGBChase-RGB\",\"cycleMS\": 1000,\"colorPattern\": \"FF000000FF000000FF\",\"enabled\": 1,\"channelSet\":\"" + std::to_string(start) + "-" + std::to_string(end) + "\",\"channelSetType\":\"channelRange\"}";
    const std::string url = "/api/testmode";
    return postData(url, data, "application/json" );
}

bool FPPController::isInTestMode() const {
    // /api/testmode
    std::string data;
    const std::string url = "/api/testmode";
    getData(url, data, "application/json" );
    Json::Value v = LoadJsonFromString(data);
    if (v["enabled"].asInt() == 1) {
        return true;
    }
    //"enabled": 0
    return false;
}

std::pair<int, int> FPPController::getPortChannels(int port) const {
    std::string data;
    const std::string sysurl ="/api/system/info";
    getData(sysurl, data, "application/json" );
    Json::Value sv = LoadJsonFromString(data);
    std::string plat = sv["Platform"].asString();
    
    if(plat.find("Beagle") == std::string::npos) {
        const std::string url2 = "/api/channel/output/co-pixelStrings";
        getData(url2, data, "application/json" );
    } else {
        const std::string url = "/api/channel/output/co-bbbStrings";
        getData(url, data, "application/json" );
    }
    Json::Value v = LoadJsonFromString(data);
    Json::Value config = v["channelOutputs"][0];
    for (int j = 0; j < config["outputs"].size(); j++) {
        Json::Value s = config["outputs"][j];

        int sport = s["portNumber"].asInt();
        if(sport == (port - 1)) {
            int startChannel{1};
            int channel_count{1};
            for (int i = 0; i < s["virtualStrings"].size(); i++) {
                Json::Value vsc = s["virtualStrings"][i];
                if(i == 0) {
                    startChannel = vsc["startChannel"].asInt() + 1;
                }
                int pixelCount = vsc["pixelCount"].asInt();
                std::string colorOrder = vsc["colorOrder"].asString();
                channel_count = channel_count + ( pixelCount * colorOrder.size());
            }
            return {startChannel, channel_count};
        }
    }
    return {1, 8};
}
