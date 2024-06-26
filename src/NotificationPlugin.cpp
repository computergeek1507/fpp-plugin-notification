#include "fpp-pch.h"

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <execution>
#include <istream>
#include <ostream>
#include <iostream>
#include <thread>
#include <vector>

#include <unistd.h>
#include <termios.h>
#include <chrono>
#include <thread>
#include <cmath>

#include "common.h"
#include "settings.h"
#include "Plugin.h"
#include "Plugins.h"
#include "log.h"

//#if __has_include("channeloutput/ChannelOutputSetup.h")
#include "channeloutput/ChannelOutputSetup.h"
//#elif __has_include("channeloutput/channeloutput.h")
//#include "channeloutput/channeloutput.h"
//#endif

#include "fppversion_defines.h"

#include "commands/Commands.h"

#include "notificationBase.h"
#include "pushover.h"
//#include "openHABItem.h"

class NotificationPlugin : public FPPPlugin {
private:

public:
    NotificationPlugin() : FPPPlugin("fpp-plugin-notification") {
        LogInfo(VB_PLUGIN, "Initializing Notification Plugin\n");
        registerCommand();
    }
    virtual ~NotificationPlugin() {
    }

    class SendEmailCommand : public Command {
    public:
        SendEmailCommand(openHABPlugin *p) : Command("Notification Send Email"), plugin(p) {
            args.push_back(CommandArg("Recipient", "string", "Recipient Email Address"));
            args.push_back(CommandArg("Subject", "string", "Email Subject"));
            args.push_back(CommandArg("Message", "string", "Email Message"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string recipient = "";
            std::string subject = "";
            std::string message = "";
             if (args.size() >= 1) {
                recipient = args[0];
            }
            if (args.size() >= 2) {
                subject = args[1];
            }
            if (args.size() >= 3) {
                message = args[2];
            }
            //plugin->SetSwitchState(ipAddress, port, item, bulbOn);
            return std::make_unique<Command::Result>("openHAB Switch Set");
        }
        NotificationPlugin *plugin;
    };

    

    void registerCommand() {
        CommandManager::INSTANCE.addCommand(new openHABSetSwitchCommand(this));
    }


    virtual void modifySequenceData(int ms, uint8_t *seqData) override {
        try
        {
            //sendChannelData(seqData);
        }
        catch(std::exception const& ex)
        {
            std::cout << ex.what();
        }
    }

    virtual void playlistCallback(const Json::Value &playlist, const std::string &action, const std::string &section, int item) {
        if (settings["Start"] == "PlaylistStart" && action == "start") {
            //EnableTPLinkItems();
        }  
    }

    void SetSwitchState(std::string const& ip, uint16_t port, std::string const& item, bool state) {

        auto SetSwState = [ip,port,state](std::string const& __item)
        {
            openHABSwitch openSwitch(ip, port, __item, 1);
            if(state){
                openSwitch.setSwitchOn();
            } else{
                openSwitch.setSwitchOff();
            }
        };
        if(item.find(",") != std::string::npos) {
            auto items = split(item, ',');
            for(auto const& item_ : items) {
               SetSwState(item_);
            }
        } else {
            SetSwState(item);
        }
    }
};

extern "C" {
    FPPPlugin *createPlugin() {
        return new NotificationPlugin();
    }
}
