#include "pushbullet.h"

#include "fpp-pch.h"
#include "common.h"
#include "settings.h"

#include <stdlib.h>
#include <cstdint>
#include <thread>
#include <cmath>

pushbullet::pushbullet(std::string const& ip) :
    notificationBase()
{
}

pushbullet::~pushbullet() {

}

