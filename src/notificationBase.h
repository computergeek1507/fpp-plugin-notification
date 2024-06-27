#pragma once

#include <stdlib.h>
#include <list>
#include <string>
#include <atomic>

#include "log.h"

#include <curl/curl.h>

class NotificationBase {
public:
    NotificationBase( );
    virtual ~NotificationBase();

   

    virtual std::string GetType() const = 0;

    virtual bool SendMessage(std::string const& message) = 0;

protected:

    std::atomic<bool> m_unreachable;
    std::atomic<bool> m_issending;

    CURL *m_curl;

    void sendCmd( std::string const& message);

private:

};