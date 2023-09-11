#pragma once

#include "controller_base.h"

#include <string>

struct WLEDController : ControllerBase{
    explicit WLEDController(std::string const& ip);
    virtual ~WLEDController();

    std::string GetType() const override {return "WLED";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;

    bool isInTestMode() const override;
};