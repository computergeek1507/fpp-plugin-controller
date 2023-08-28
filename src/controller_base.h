#pragma once

#include <stdlib.h>
#include <list>
#include <string>
#include <atomic>

//#include "common.h"
#include "log.h"

class ControllerBase {
public:
    ControllerBase(std::string const& ip, unsigned int output_count );
    virtual ~ControllerBase();

    std::string GetIPAddress() const { return m_ipAddress; }
    virtual std::string GetType() const = 0;

    virtual bool setTestModeOn( int outputs )const = 0;
    virtual bool setTestModeOff( int outputs )const = 0;

    virtual std::string GetConfigString() const {
        return "IP: " + GetIPAddress() + " Device Type: " + GetType();
    }

protected:
    std::string m_ipAddress;
    uint16_t m_outputs;

    std::string sendCmd(std::string const& cmd);

private:


};