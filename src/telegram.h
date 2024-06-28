#pragma once

#include "notificationBase.h"

class Telegram : public NotificationBase{
public:
    Telegram(std::string const& bot_token, std::string const& chat_id );
    virtual ~Telegram();

    std::string GetType() const override {return "Telegram";}

    bool SendMessage(std::string const& message) override;
private:
    std::string m_bot_token;
    std::string m_chat_id;
};