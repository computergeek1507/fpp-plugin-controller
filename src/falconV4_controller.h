#pragma once

#include "controller_base.h"

#include <string>

class FalconV4Controller : public ControllerBase{
public:
    FalconV4Controller(std::string const& ip, unsigned int output_count );
    virtual ~FalconV4Controller();

    std::string GetType() const override {return "FalconV4";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;

    bool isInTestMode() const override;

private:

};