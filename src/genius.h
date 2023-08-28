#pragma once

#include "controller_base.h"

class Genius : public ControllerBase{
public:
    Genius(std::string const& ip, unsigned int output_countm );
    virtual ~Genius();

    std::string GetType() const override {return "Genius";}

    bool setTestModeOn( int outputs )const override;
    bool setTestModeOff( int outputs )const override;

private:

};