#pragma once

#include "controller_base.h"

#include <string>

class WLEDController : public ControllerBase{
public:
    WLEDController(std::string const& ip, unsigned int output_count );
    virtual ~WLEDController();

    std::string GetType() const override {return "WLED";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;

    bool isInTestMode() const override;

private:

};