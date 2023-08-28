#pragma once

#include "controller_base.h"

#include <string>

class FalconV4 : public ControllerBase{
public:
    FalconV4(std::string const& ip, unsigned int output_countm );
    virtual ~FalconV4();

    std::string GetType() const override {return "FalconV4";}

    bool setTestModeOn( int outputs )const override;
    bool setTestModeOff( int outputs )const override;

private:

};