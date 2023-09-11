#pragma once

#include "controller_base.h"

#include <string>

struct GeniusController : ControllerBase{
    explicit GeniusController(std::string const& ip);
    virtual ~GeniusController();

    std::string GetType() const override {return "Genius";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;

    bool isInTestMode() const override;
};