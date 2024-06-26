#pragma once

#include "notificationBase.h"

class pushover : public notificationBase{
public:
    pushover(std::string const& key );
    virtual ~pushover();



private:

};