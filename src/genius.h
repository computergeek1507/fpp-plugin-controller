#pragma once

#include "controller_base.h"

class Genius : public ControllerBase{
public:
    Genius(std::string const& ip, unsigned int output_count );
    virtual ~Genius();

    std::string GetType() const override {return "Genius";}

    bool setTestModeOn( )const override;
    bool setTestModeOff( )const override;

private:

};