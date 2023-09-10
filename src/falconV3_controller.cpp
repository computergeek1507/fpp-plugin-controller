#include "falconV3_controller.h"

#include "fpp-pch.h"
#include "common.h"
#include "settings.h"

FalconV3Controller::FalconV3Controller(std::string const& ip, unsigned int output_count) :
    ControllerBase(ip,output_count)
{

}

FalconV3Controller::~FalconV3Controller() {

}

bool FalconV3Controller::setTestModeOn() const {
    //t=1&m=5&e0=1&e1=1&e2=1&e3=1&e4=1&e5=1&e6=1&e7=1&e8=1&e9=1&e10=1&e11=1&e12=1&e13=1&e14=1&e15=1&e16=1&e17=1&e18=1&e19=1&e20=1&e21=1&e22=1&e23=1&e24=1&e25=1&e26=1&e27=1&e28=1&e29=1&e30=1&e31=1&e32=1&e33=1&e34=1&e35=1&e36=1&e37=1&e38=1&e39=1&e40=1&e41=1&e42=1&e43=1&e44=1&e45=1&e46=1&e47=1&e48=1&e49=1&e50=1&e51=1&s0=1&s1=1&s2=1&s3=1
    std::string cmd = "t=1&m=5";
    for (int j = 0; j < m_outputs; j++) {
        cmd += "&e" + std::to_string(j) + "=1";
    }
    cmd += "&s0=1&s1=1&s2=1&s3=1";
    return postData("/test.htm", cmd, "application/x-www-form-urlencoded; charset=UTF-8");
}

bool FalconV3Controller::setTestModeOff() const {
    //t=0&m=5&e0=1&e1=1&e2=1&e3=1&e4=1&e5=1&e6=1&e7=1&e8=1&e9=1&e10=1&e11=1&e12=1&e13=1&e14=1&e15=1&e16=1&e17=1&e18=1&e19=1&e20=1&e21=1&e22=1&e23=1&e24=1&e25=1&e26=1&e27=1&e28=1&e29=1&e30=1&e31=1&e32=1&e33=1&e34=1&e35=1&e36=1&e37=1&e38=1&e39=1&e40=1&e41=1&e42=1&e43=1&e44=1&e45=1&e46=1&e47=1&e48=1&e49=1&e50=1&e51=1&s0=1&s1=1&s2=1&s3=1
    const std::string cmd = "t=0&m=5";
    return postData("/test.htm", cmd, "application/x-www-form-urlencoded; charset=UTF-8");
}

bool FalconV3Controller::isInTestMode() const 
{
    //strings.xml
    std::string data;
    bool worked = getData("/strings.xml", data, "application/text");

    if(data.find("t='1'") != std::string::npos) {
        return true;
    }
    //t="0"
    return false;
}

