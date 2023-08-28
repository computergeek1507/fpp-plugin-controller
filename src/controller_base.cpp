#include "controller_base.h"

#include <iostream>
#include <istream>
#include <ostream>

ControllerBase::ControllerBase(std::string const& ip, unsigned int output_count) :
    m_ipAddress(ip),
    m_outputs(output_count),
{
}

std::string ControllerBase::sendCmd(std::string const& cmd) {
    try {
       return std::string("");
    }
    catch(std::exception const& ex) {
        LogInfo(VB_PLUGIN, "Error %s \n", ex.what());
    }
    return std::string("");
}


