#pragma once

#include "notificationBase.h"

class Pushover : public NotificationBase{
public:
    Pushover(std::string const& token, std::string const& user );
    virtual ~Pushover();

    std::string GetType() const override {return "Pushover";}

    bool SendMessage(std::string const& message) override;

private:
std::string m_token;
std::string m_user;

};