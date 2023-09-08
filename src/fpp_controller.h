#pragma once

#include "controller_base.h"

class FPPController : public ControllerBase{
public:
    FPPController(std::string const& ip, unsigned int output_count );
    virtual ~FPPController();

    std::string GetType() const override {return "FPP";}

    bool setTestModeOn() const override;
    bool setTestModeOff() const override;

    bool isInTestMode() const override;

private:

};