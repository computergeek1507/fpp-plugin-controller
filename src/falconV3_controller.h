#pragma once

#include "controller_base.h"

#include <string>

struct FalconV3Controller : ControllerBase{
    explicit FalconV3Controller(std::string const& ip);
    virtual ~FalconV3Controller();

    std::string GetType() const override {return "FalconV3";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;
    bool setTestModeOnPort(int portNum) const override;

    bool isInTestMode() const override;

    int getStringCount() const;
};