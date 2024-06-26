#pragma once

#include "notificationBase.h"

#include <string>

class pushbullet : public notificationBase{
public:
    pushbullet(std::string const& ip );
    virtual ~pushbullet();

    std::string GetType() const override {return "Switch";}

private:

};