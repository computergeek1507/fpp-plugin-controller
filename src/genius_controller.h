#pragma once

#include "controller_base.h"

class GeniusController : public ControllerBase{
public:
    GeniusController(std::string const& ip, unsigned int output_count );
    virtual ~GeniusController();

    std::string GetType() const override {return "Genius";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;

    bool isInTestMode() const override;

private:

};