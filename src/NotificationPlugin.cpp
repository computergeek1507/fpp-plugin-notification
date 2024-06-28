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
#include "Warnings.h"

//#if __has_include("channeloutput/ChannelOutputSetup.h")
#include "channeloutput/ChannelOutputSetup.h"
//#elif __has_include("channeloutput/channeloutput.h")
//#include "channeloutput/channeloutput.h"
//#endif

#include "fppversion_defines.h"

#include "commands/Commands.h"

#include "notificationBase.h"
#include "pushover.h"
#include "telegram.h"

class NotificationWarningListener : public WarningListener {
public:
    NotificationWarningListener(std::function<void(std::list<std::string>&)> callback): m_callback(callback)  {}
    virtual ~NotificationWarningListener() {}

    virtual void handleWarnings(std::list<std::string>& warnings) {
        if(m_callback)
        {
            m_callback(warnings);
        }
    }
    std::function<void(std::list<std::string>&)> m_callback;
};

//static NotificationWarningListener* NOTIFICATION_WARNING_LISTENER = nullptr;


class NotificationPlugin : public FPPPlugin {
public:
    std::vector<std::unique_ptr<NotificationBase>> m_notification_services;

    std::list<std::string> m_warnings;

    NotificationPlugin() : FPPPlugin("fpp-plugin-notification") {
        LogInfo(VB_PLUGIN, "Initializing Notification Plugin\n");
        registerCommand();
        ReadSettings();

        auto add_warnings{ [this](std::list<std::string>& warnings) mutable {
            m_warnings = warnings;
            ReadWarnings();
        } };

       // if (NOTIFICATION_WARNING_LISTENER == nullptr) {
        //    NOTIFICATION_WARNING_LISTENER = new NotificationWarningListener(add_warnings);
        //    WarningHolder::AddWarningListener(NOTIFICATION_WARNING_LISTENER);
       // }
    }
    virtual ~NotificationPlugin() {
       // if (NOTIFICATION_WARNING_LISTENER != nullptr) {
        //    WarningHolder::RemoveWarningListener(NOTIFICATION_WARNING_LISTENER);
       //    delete NOTIFICATION_WARNING_LISTENER;
       //     NOTIFICATION_WARNING_LISTENER = nullptr;
       // }
    }

    void ReadWarnings() 
    {
        for(auto const& warning : m_warnings)
        {
            SendMessage(warning);
        }
    }

    void ReadSettings() {
        if (FileExists(FPP_DIR_CONFIG("/plugin.notification.json"))) {
            try {
                Json::Value root;
                bool success =  LoadJsonFromFile(FPP_DIR_CONFIG("/plugin.notification.json"), root);
                if(!success){
                    return;
                }

                if (root.isMember("pushover")) {
                    
                   std::string token = root["pushover"]["token"].asString();
                   std::string user = root["pushover"]["user"].asString();
                   if(!token.empty() && !user.empty()){
                    m_notification_services.push_back(std::move(std::make_unique<Pushover>(token, user)));
                    LogInfo(VB_PLUGIN, "Added Pushover Service\n");
                   }
                }
                if (root.isMember("telegram")) {
                    
                   std::string bottoken = root["telegram"]["bottoken"].asString();
                   std::string chatid = root["telegram"]["chatid"].asString();
                   if(!bottoken.empty() && !chatid.empty()){
                    m_notification_services.push_back(std::move(std::make_unique<Telegram>(bottoken, chatid)));
                    LogInfo(VB_PLUGIN, "Added Telegram Service\n");
                   }
                }
                
                LogInfo(VB_PLUGIN, "Notification Started\n");
            } catch (...) {
                LogErr(VB_PLUGIN, "Could not Read Notification Plugin JSON\n");
            }                
        }else{
            LogInfo(VB_PLUGIN, "No plugin.notification.json config file found\n");
        }
    }

    class SendEmailCommand : public Command {
    public:
        SendEmailCommand(NotificationPlugin *p) : Command("Notification Send Email"), plugin(p) {
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
            return std::make_unique<Command::Result>("Email Sent");
        }
        NotificationPlugin *plugin;
    };

    class SendPushoverCommand : public Command {
    public:
        SendPushoverCommand(NotificationPlugin *p) : Command("Notification Send Pushover"), plugin(p) {
            args.push_back(CommandArg("Token", "string", "App Token Key"));
            args.push_back(CommandArg("User", "string", "User Key"));
            args.push_back(CommandArg("Message", "string", "Message"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string token = "";
            std::string user = "";
            std::string message = "";
             if (args.size() >= 1) {
                token = args[0];
            }
            if (args.size() >= 2) {
                user = args[1];
            }
            if (args.size() >= 3) {
                message = args[2];
            }
            plugin->SendPushover(token, user, message);
            return std::make_unique<Command::Result>("Pushover Sent");
        }
        NotificationPlugin *plugin;
    };

    class SendMessageCommand : public Command {
    public:
        SendMessageCommand(NotificationPlugin *p) : Command("Notification Send Message"), plugin(p) {
            args.push_back(CommandArg("Message", "string", "Message"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string message = "";
             if (args.size() >= 1) {
                message = args[0];
            }           
            plugin->SendMessage( message);
            return std::make_unique<Command::Result>("Message Sent");
        }
        NotificationPlugin *plugin;
    };

    void registerCommand() {
        CommandManager::INSTANCE.addCommand(new SendEmailCommand(this));
        CommandManager::INSTANCE.addCommand(new SendPushoverCommand(this));
        CommandManager::INSTANCE.addCommand(new SendMessageCommand(this));
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

    void SendPushover(std::string const& token, std::string const& user, std::string const& message) {

        Pushover push(token, user);
        push.SendMessage(message);
        /*auto SetSwState = [ip,port,state](std::string const& __item)
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
        }*/
    }

    void SendMessage(std::string const& message) {
        for(auto const& service : m_notification_services)
        {
            service->SendMessage(message);
        }
        //std::for_each(std::execution::par, std::begin(m_notification_services), std::end(m_notification_services), [message](auto& service) {
        //    service->SendMessage(message);
        //});
    }
};

extern "C" {
    FPPPlugin *createPlugin() {
        return new NotificationPlugin();
    }
}
