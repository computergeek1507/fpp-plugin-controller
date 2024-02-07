#pragma once

#include "controller_base.h"

#include <string>

struct FPPController : ControllerBase{
    explicit FPPController(std::string const& ip);
    virtual ~FPPController();

    std::string GetType() const override {return "FPP";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;
    bool setTestModeOnPort(int portNum) const override;

    bool isInTestMode() const override;

    std::pair<int, int> getPortChannels(int port) const;
};