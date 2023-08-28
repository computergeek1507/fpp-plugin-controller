#pragma once

#include "controller_base.h"

#include <string>

class FalconV4 : public ControllerBase{
public:
    FalconV4(std::string const& ip, unsigned int output_count );
    virtual ~FalconV4();

    std::string GetType() const override {return "FalconV4";}

    bool setTestModeOn( )const override;
    bool setTestModeOff( )const override;

private:

};