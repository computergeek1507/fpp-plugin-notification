#pragma once

#include "notificationBase.h"

class Telegram : public NotificationBase{
public:
    Telegram(std::string const& key, std::string const& clientID );
    virtual ~Telegram();


    std::string GetType() const override {return "Telegram";}

private:

};