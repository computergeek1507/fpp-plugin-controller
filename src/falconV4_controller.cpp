#include "falconV4_controller.h"

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

FalconV4Controller::FalconV4Controller(std::string const& ip, unsigned int output_count) :
    ControllerBase(ip,output_count)
{

}

FalconV4Controller::~FalconV4Controller() {

}

bool FalconV4Controller::setTestModeOn() const {
    //{"T":"S","M":"TS","B":0,"E":16,"I":0,"P":{"E":"Y","D":"Y","S":20,"Y":1,"A":[{"P":0,"R":0,"S":0},{"P":1,"R":0,"S":0},{"P":1,"R":0,"S":0},{"P":3,"R":0,"S":0},{"P":4,"R":0,"S":0}]}}
    //{"T":"S","M":"TS","B":2,"E":16,"I":10,"P":{"E":"Y","D":"Y","S":20,"Y":1,"A":[{"P":10,"R":0,"S":0},{"P":11,"R":0,"S":0},{"P":12,"R":0,"S":0},{"P":13,"R":0,"S":0},{"P":14,"R":0,"S":0}]}}
    //{"T":"S","M":"TS","B":3,"E":16,"I":15,"P":{"E":"Y","D":"Y","S":20,"Y":1,"A":[{"P":15,"R":0,"S":0}]}}
    static constexpr int PORT_PER = 5;
    int numberOfCalls = std::ceil((double)m_outputs / PORT_PER);
    bool test = true;
    for (int j = 0; j < numberOfCalls; j++) {
        int index2 = j * PORT_PER;
        std::string cmd = "{\"T\":\"S\",\"M\":\"TS\",\"B\":" + std::to_string(j) + ",\"E\":16,\"I\":" + std::to_string(index2) + ",\"P\":{\"E\":\"Y\",\"D\":\"Y\",\"S\":20,\"Y\":1,\"A\":[";
        for (int i = 0; i < PORT_PER; i++) {
            if(index2 + i >= m_outputs) {
                break;
            }
            std::string cmd2 = "{\"P\":"+ std::to_string(index2 + i) +",\"R\":0,\"S\":0},";
            cmd += cmd2;
            
        }
        cmd.pop_back();//remove last comma
        cmd += "]}}";
        test &= postData("/api", cmd, "application/json");
    }
    
    return test;
}

bool FalconV4Controller::setTestModeOff() const {

    const std::string cmd = R"({"T":"S","M":"TS","B":0,"E":16,"I":0,"P":{"E":"N","D":"Y","S":20,"Y":1,"A":[]}})";
    return postData("/api", cmd, "application/json");
}

bool FalconV4Controller::isInTestMode() const {
    const std::string cmd = R"({"T":"Q","M":"ST","B":0,"E":0,"I":0,"P":{}})";
    std::string data;
    bool worked = postData("/api", cmd, data, "application/json");

    if(data.find("\"TS\"") != std::string::npos && data.find("\"TS\":0") != std::string::npos) {
        return false;
    }
    if(data.find("\"TS\"") != std::string::npos) {
        return true;
    }
    //""TS":0"
    return false;
}

