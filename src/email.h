#pragma once

#include "notificationBase.h"

class Email : public NotificationBase{
public:
    Email(std::string const& server, uint16_t port, std::string const& username, std::string const& password, bool tls );
    virtual ~Email();

    std::string GetType() const override {return "Email";}

    bool SendMessage(std::string const& message) override;
private:

std::string const& m_server;
uint16_t m_port{587};
std::string const& m_username;
std::string const& m_password;

};