#pragma once

#include "controller_base.h"

#include <string>

struct FalconV4Controller : ControllerBase{
    explicit FalconV4Controller(std::string const& ip);
    virtual ~FalconV4Controller();

    std::string GetType() const override {return "FalconV4";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;
    bool setTestModeOnPort(int portNum) const override;

    bool isInTestMode() const override;
    
    int getStringCount() const;
};