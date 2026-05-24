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
    std::string data = "{\"mode\":\"RGBChase\",\"subMode\":\"RGBChase-RGB\",\"cycleMS\": 1000,\"colorPattern\": \"FF000000FF000000FF\",\"enabled\": 1,\"channelSet\": \"*\",\"channelSetType\": \"channelRange\"}";
    const std::string url = "/api/testmode";
    return postData(url, data, "application/json" );
}

bool FPPController::setTestModeOff() const {
    std::string data= "{\"mode\":\"RGBChase\",\"subMode\":\"RGBChase-RGB\",\"cycleMS\": 1000,\"colorPattern\": \"FF000000FF000000FF\",\"enabled\": 0,\"channelSet\": \"*\",\"channelSetType\": \"channelRange\"}";
    const std::string url = "/api/testmode";
    return postData(url, data, "application/json" );
}

bool FPPController::setTestModeOnPort(int portNum) const {
CURL *curl;
CURLcode res;
curl = curl_easy_init();
if(curl) {
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
  curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.5.137/api/testmode");
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  const char *data = "{\r\n    \"mode\": \"RGBChase\",\r\n    \"subMode\": \"RGBChase-RGB\",\r\n    \"cycleMS\": 1000,\r\n    \"colorPattern\": \"FF000000FF000000FF\",\r\n    \"enabled\": 0,\r\n    \"channelSet\": \"1-301\",\r\n    \"channelSetType\": \"channelRange\"\r\n}";
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
  res = curl_easy_perform(curl);
  return true;
}
curl_easy_cleanup(curl);
    //auto [ start, end ] = getPortChannels(portNum);
    //std::string data= "{\"mode\":\"RGBChase\",\"subMode\":\"RGBChase-RGB\",\"cycleMS\": 1000,\"colorPattern\": \"FF000000FF000000FF\",\"enabled\": 1,\"channelSet\": \"" + std::to_string(start) + "-" + std::to_string(end-1) + "\",\"channelSetType\": \"channelRange\"}";
    //const std::string url = "/api/testmode";
    //return postData(url, data, "application/json" );
}

bool FPPController::isInTestMode() const {
    // /api/testmode
    std::string data;
    const std::string url = "/api/testmode";
    getData(url, data, "" );
    Json::Value v = LoadJsonFromString(data);
    if (v["enabled"].asInt() == 1) {
        return true;
    }
    //"enabled": 0
    return false;
}

std::pair<int, int> FPPController::getPortChannels(int port) const {
    LogInfo(VB_PLUGIN, "controller port '%d'\n", port);
    std::string data;
    const std::string sysurl ="/api/system/info";
    getData(sysurl, data, "" );
    Json::Value sv = LoadJsonFromString(data);
    std::string plat = sv["Platform"].asString();
    LogInfo(VB_PLUGIN, "Platform '%s'\n",plat.c_str());
    if(plat.find("Beagle") == std::string::npos) {
        const std::string url2 = "/api/channel/output/co-pixelStrings";
        getData(url2, data, "" );
    } else {
        const std::string url = "/api/channel/output/co-bbbStrings";
        getData(url, data, "" );
    }
    Json::Value v = LoadJsonFromString(data);
    LogInfo(VB_PLUGIN, "channelOutputs size '%d'\n", v["channelOutputs"].size());

    Json::Value config = v["channelOutputs"][0];

    LogInfo(VB_PLUGIN, "outputs size '%d'\n", config["outputs"].size());
    for (int j = 0; j < config["outputs"].size(); j++) {
        Json::Value s = config["outputs"][j];

        int sport = s["portNumber"].asInt();
        LogInfo(VB_PLUGIN, "portNumber '%d'\n", sport);
        if(sport == (port - 1)) {
            int startChannel{1};
            int channel_count{1};
            for (int i = 0; i < s["virtualStrings"].size(); i++) {
                Json::Value vsc = s["virtualStrings"][i];
                if(i == 0) {
                    startChannel = vsc["startChannel"].asInt() + 1;
                    LogInfo(VB_PLUGIN, "startChannel '%d'\n", startChannel);
                }
                int pixelCount = vsc["pixelCount"].asInt();
                std::string colorOrder = vsc["colorOrder"].asString();
                LogInfo(VB_PLUGIN, "pixelCount '%d'\n", pixelCount);
                LogInfo(VB_PLUGIN, "colorOrder '%s'\n", colorOrder.c_str());
                channel_count = channel_count + ( pixelCount * colorOrder.size());
            }
            LogInfo(VB_PLUGIN, "startChannel '%d'\n", startChannel);
            LogInfo(VB_PLUGIN, "channel_count '%d'\n", channel_count);
            return {startChannel, channel_count};
        }
    }
    return {1, 8};
}
