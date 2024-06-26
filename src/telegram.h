#pragma once

#include "notificationBase.h"

class telegram : public notificationBase{
public:
    telegram(std::string const& key, std::string const& clientID );
    virtual ~telegram();


    std::string GetType() const override {return "Light";}

private:

};