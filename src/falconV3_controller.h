#pragma once

#include "controller_base.h"

#include <string>

class FalconV3Controller : public ControllerBase{
public:
    FalconV3Controller(std::string const& ip, unsigned int output_count );
    virtual ~FalconV3Controller();

    std::string GetType() const override {return "FalconV3";}

    bool setTestModeOn( )const override;
    bool setTestModeOff( )const override;

private:

};