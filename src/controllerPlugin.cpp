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

#include <httpserver.hpp>
#include "common.h"
#include "settings.h"
#include "Plugin.h"
#include "Plugins.h"
#include "log.h"
#include "MultiSync.h"

#include "fppversion_defines.h"

#include "commands/Commands.h"

#include "genius_controller.h"
#include "falconV4_controller.h"
#include "falconV3_controller.h"
#include "fpp_controller.h"
#include "controller_base.h"

class ControllerPlugin : public FPPPlugin, public httpserver::http_resource {
private:

public:
    ControllerPlugin() : FPPPlugin("fpp-plugin-ControllerPlugin") {
        LogInfo(VB_PLUGIN, "Initializing Controller Plugin\n");
        //readFiles();
        registerCommand();
    }
    virtual ~ControllerPlugin() {

    }

    class ControllerTestOnCommand : public Command {
    public:
        ControllerTestOnCommand(ControllerPlugin *p) : Command("Controller Set Test Mode On"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address"));
            args.push_back(CommandArg("type", "string", "Controller Type").setContentList({"FalconV4", "Genius", "FPP", "FalconV3"}).setDefaultValue("FalconV4"));
            args.push_back(CommandArg("ports", "int", "Set Number of Ports").setRange(0, 128).setDefaultValue("16"));
            args.push_back(CommandArg("check", "bool", "Only do Request If IP is in Multisync List").setDefaultValue("false"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";
            std::string type = "FalconV4";
            int outputs = 16;
            bool checkList = false;
            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            if (args.size() >= 2) {
                type = args[1];
            }
            if (args.size() >= 3) {
                outputs = std::stoi(args[2]);
            }
            if (args.size() >= 4) {
                checkList = args[3]=="true";
            }
            plugin->SetControllerTestModeOn(ipAddress, type, outputs, checkList);
            return std::make_unique<Command::Result>("Controller Test Mode On");
        }
        ControllerPlugin *plugin;
    };      

    class ControllerTestOffCommand : public Command {
    public:
        ControllerTestOffCommand(ControllerPlugin *p) : Command("Controller Set Test Mode Off"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address")); 
            args.push_back(CommandArg("type", "string", "Controller Type").setContentList({"FalconV4", "Genius", "FPP", "FalconV3"}).setDefaultValue("FalconV4"));
            args.push_back(CommandArg("check", "bool", "Only do Request If IP is in Multisync List").setDefaultValue("false"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";
            std::string type = "FalconV4";
            int outputs = 16;//not used
            bool checkList = false;
            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            if (args.size() >= 2) {
                type = args[1];
            }
            if (args.size() >= 3) {
                checkList = args[2]=="true";
            }
            plugin->SetControllerTestModeOff(ipAddress, type, outputs, checkList);
            return std::make_unique<Command::Result>("Controller Test Mode Off");
        }
        ControllerPlugin *plugin;
    };

    class ControllerTestOnAutoCommand : public Command {
    public:
        ControllerTestOnAutoCommand(ControllerPlugin *p) : Command("Controller Set Test Mode On Auto"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";

            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            
            plugin->SetControllerTestModeOn(ipAddress);
            return std::make_unique<Command::Result>("Controller Test Mode On Auto");
        }
        ControllerPlugin *plugin;
    };      

    class ControllerTestOffAutoCommand : public Command {
    public:
        ControllerTestOffAutoCommand(ControllerPlugin *p) : Command("Controller Set Test Mode Off Auto"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address")); 
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";

            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            plugin->SetControllerTestModeOff(ipAddress);
            return std::make_unique<Command::Result>("Controller Test Mode Off Auto");
        }
        ControllerPlugin *plugin;
    };

    void registerCommand() {
        CommandManager::INSTANCE.addCommand(new ControllerTestOnCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerTestOffCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerTestOnCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerTestOffCommand(this));
    }

    std::unique_ptr<ControllerBase> MakeController(std::string const& ip, std::string const& type, int outputs) {
        if (type.find("FalconV4") != std::string::npos) {
            return std::move(std::make_unique<FalconV4Controller>(ip, outputs));
        } else if (type.find("Genius") != std::string::npos) {
            return std::move(std::make_unique<GeniusController>(ip, outputs));
        } else if (type.find("FPP") != std::string::npos) {
            return std::move(std::make_unique<FPPController>(ip, outputs));
        } else if (type.find("FalconV3") != std::string::npos) {
            return std::move(std::make_unique<FalconV3Controller>(ip, outputs));
        } else {
            LogInfo(VB_PLUGIN, "controller type not found '%s'", type.c_str());
        }
        return std::move(std::make_unique<FalconV4Controller>(ip, outputs));
    }

    std::unique_ptr<ControllerBase> MakeController(std::string const& ip, MultiSyncSystemType const& type, int outputs) {
        if (0x88 == type || 0x89 == type) {
            return std::move(std::make_unique<FalconV4Controller>(ip, outputs));
        } else if (0xA0 <= type && 0xAA > type) {
            return std::move(std::make_unique<GeniusController>(ip, outputs));
        } else if (0x00 < type && 0x80 > type) {
            return std::move(std::make_unique<FPPController>(ip, outputs));
        } else if (0x85 == type || 0x87 == type) {
            return std::move(std::make_unique<FalconV3Controller>(ip, outputs));
        } else {
            LogInfo(VB_PLUGIN, "controller type not found '%d'", type);
        }
        return std::move(std::make_unique<FalconV4Controller>(ip, outputs));
    }

    void SetControllerTestModeOn(std::string const& ip, std::string const& type, int outputs, bool checkMulti) {
        if(ip.find(",") != std::string::npos) {
            auto ips = split(ip, ',');
            for(auto const& ip_ : ips) {
                if(checkMulti && !IsInMultiSyncList(ip_)) {
                    continue;
                }
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type, outputs);
                controllerItem->setTestModeOn();
            }
        } else {
            if(checkMulti && !IsInMultiSyncList(ip)) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type, outputs);
            controllerItem->setTestModeOn();
        }
    }

    void SetControllerTestModeOff(std::string const& ip, std::string const& type, int outputs, bool checkMulti) {
        if(ip.find(",") != std::string::npos) {
            auto ips = split(ip, ',');
            for(auto const& ip_ : ips) {
                if(checkMulti && !IsInMultiSyncList(ip_)) {
                    continue;
                }
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type, outputs);
                controllerItem->setTestModeOff();
            }
        } else {
            if(checkMulti && !IsInMultiSyncList(ip)) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type, outputs);
            controllerItem->setTestModeOff();
        }
    }

    void SetControllerTestModeOn(std::string const& ip) {
        if(ip.find(",") != std::string::npos) {
            auto ips = split(ip, ',');
            for(auto const& ip_ : ips) {
                auto type = GetMultiSyncType(ip);
            if(type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type, 16);
                controllerItem->setTestModeOn();
            }
        } else {
            auto type = GetMultiSyncType(ip);
            if(type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type, 16);
            controllerItem->setTestModeOn();
        }
    }

    void SetControllerTestModeOff(std::string const& ip) {
        if(ip.find(",") != std::string::npos) {
            auto ips = split(ip, ',');
            for(auto const& ip_ : ips) {
                auto type = GetMultiSyncType(ip);
                if(type == MultiSyncSystemType::kSysTypeUnknown) {
                    return;
                }
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type, 16);
                controllerItem->setTestModeOff();
            }
        } else {
            auto type = GetMultiSyncType(ip);
            if(type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type, 16);
            controllerItem->setTestModeOff();
        }
    }

    bool IsInMultiSyncList(std::string const& ip)
    {
        for (auto system : multiSync->GetLocalSystems()){
            if(system.address == ip){
                return true;
            }
        }
        LogInfo(VB_PLUGIN, "IP Address not found in Multisync local list '%s'", ip.c_str());
        return false;
    }

    MultiSyncSystemType GetMultiSyncType(std::string const& ip)
    {
        for (auto system : multiSync->GetLocalSystems()){
            if(system.address == ip){
                return system.type;
            }
        }
        LogInfo(VB_PLUGIN, "IP Address not found in Multisync local list '%s'", ip.c_str());
        return MultiSyncSystemType::kSysTypeUnknown;
    }
};

extern "C" {
    FPPPlugin *createPlugin() {
        return new ControllerPlugin();
    }
}
