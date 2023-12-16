#include "fpp-pch.h"

#include <string>
#include <memory>

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
#include "wled_controller.h"
#include "controller_base.h"

#define CONTROLLER_TYPES {"FalconV4", "Genius", "FPP", "FalconV3", "WLED"}

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
        ControllerTestOnCommand(ControllerPlugin *p) : Command("Controller Set Test Mode On Type",
        "Set Controller Test Modes On"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address"));
            args.push_back(CommandArg("type", "string", "Controller Type").setContentList(CONTROLLER_TYPES).setDefaultValue("FalconV4"));
            args.push_back(CommandArg("check", "bool", "Only do Request If IP is in Multisync List").setDefaultValue("false"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";
            std::string type = "FalconV4";
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
            plugin->SetControllerTestModeOn(ipAddress, type, checkList);
            return std::make_unique<Command::Result>("Controller Test Mode On");
        }
        ControllerPlugin *plugin;
    };      

    class ControllerTestOffCommand : public Command {
    public:
        ControllerTestOffCommand(ControllerPlugin *p) : Command("Controller Set Test Mode Off Type",
        "Set Controller Test Modes Off"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address")); 
            args.push_back(CommandArg("type", "string", "Controller Type").setContentList(CONTROLLER_TYPES).setDefaultValue("FalconV4"));
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
            plugin->SetControllerTestModeOff(ipAddress, type, checkList);
            return std::make_unique<Command::Result>("Controller Test Mode Off");
        }
        ControllerPlugin *plugin;
    };

    class ControllerTestOnAutoCommand : public Command {
    public:
        ControllerTestOnAutoCommand(ControllerPlugin *p) : Command("Controller Set Test Mode On",
        "Set Controller Test Modes On, using Multisync List for Controller Type"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";

            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            
            plugin->SetControllerTestModeOn(ipAddress);
            return std::make_unique<Command::Result>("Controller Test Mode On");
        }
        ControllerPlugin *plugin;
    };

    class ControllerTestOffAutoCommand : public Command {
    public:
        ControllerTestOffAutoCommand(ControllerPlugin *p) : Command("Controller Set Test Mode Off",
        "Set Controller Test Modes Off, using Multisync List for Controller Type"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address")); 
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";

            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            plugin->SetControllerTestModeOff(ipAddress);
            return std::make_unique<Command::Result>("Controller Test Mode Off");
        }
        ControllerPlugin *plugin;
    };

    class ControllerToggleTestCommand : public Command {
    public:
        ControllerToggleTestCommand(ControllerPlugin *p) : Command("Controller Toggle Test Mode Type",
        "Toggle Controller Test Modes off or on"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address"));
            args.push_back(CommandArg("type", "string", "Controller Type").setContentList(CONTROLLER_TYPES).setDefaultValue("FalconV4"));
            args.push_back(CommandArg("check", "bool", "Only do Request If IP is in Multisync List").setDefaultValue("false"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";
            std::string type = "FalconV4";
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
            plugin->ToggleControllerTestMode(ipAddress, type, checkList);
            return std::make_unique<Command::Result>("Controller Toggle Test Mode");
        }
        ControllerPlugin *plugin;
    };

    class ControllerToggleTestAutoCommand : public Command {
    public:
        ControllerToggleTestAutoCommand(ControllerPlugin *p) : Command("Controller Toggle Test Mode",
        "Toggle Controller Test Modes off or on, using Multisync List for Controller Type"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";

            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            
            plugin->ToggleControllerTestMode(ipAddress);
            return std::make_unique<Command::Result>("Controller Toggle Test Mode");
        }
        ControllerPlugin *plugin;
    }; 

    class ControllerSetAllTestModeOn : public Command {
    public:
        ControllerSetAllTestModeOn(ControllerPlugin *p) : Command("Controller Set All Test Mode On",
        "Set All Controller Test Modes On using Multisync List"), plugin(p) {
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {                        
            plugin->SetAllControllerTestModeOn();
            return std::make_unique<Command::Result>("Controller Set All Test Mode On");
        }
        ControllerPlugin *plugin;
    };

    class ControllerSetAllTestModeOff : public Command {
    public:
        ControllerSetAllTestModeOff(ControllerPlugin *p) : Command("Controller Set All Test Mode Off",
        "Set All Controller Test Modes Off using Multisync List"), plugin(p) {
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {                        
            plugin->SetAllControllerTestModeOff();
            return std::make_unique<Command::Result>("Controller Set All Test Mode Off");
        }
        ControllerPlugin *plugin;
    };


    void registerCommand() {
        CommandManager::INSTANCE.addCommand(new ControllerTestOnCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerTestOnAutoCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerTestOffCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerTestOffAutoCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerToggleTestCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerToggleTestAutoCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerSetAllTestModeOn(this));
        CommandManager::INSTANCE.addCommand(new ControllerSetAllTestModeOff(this));
    }

    std::unique_ptr<ControllerBase> MakeController(std::string const& ip, std::string const& type) {
        if (type.find("FalconV4") != std::string::npos) {
            return std::move(std::make_unique<FalconV4Controller>(ip));
        } else if (type.find("Genius") != std::string::npos) {
            return std::move(std::make_unique<GeniusController>(ip));
        } else if (type.find("FPP") != std::string::npos) {
            return std::move(std::make_unique<FPPController>(ip));
        } else if (type.find("FalconV3") != std::string::npos) {
            return std::move(std::make_unique<FalconV3Controller>(ip));
        } else if (type.find("WLED") != std::string::npos) {
            return std::move(std::make_unique<WLEDController>(ip));
        } else {
            LogInfo(VB_PLUGIN, "controller type not found '%s'\n", type.c_str());
        }
        return nullptr;
    }

    std::unique_ptr<ControllerBase> MakeController(std::string const& ip, MultiSyncSystemType const& type) {
        if (0x88 == type || 0x89 == type) {
            return std::move(std::make_unique<FalconV4Controller>(ip));
        } else if (0xA0 <= type && 0xAF >= type) {
            return std::move(std::make_unique<GeniusController>(ip));
        } else if (0x00 < type && 0x80 > type) {
            return std::move(std::make_unique<FPPController>(ip));
        } else if (0x85 == type || 0x87 == type) {
            return std::move(std::make_unique<FalconV3Controller>(ip));
        } else if (0xFB == type) {
            return std::move(std::make_unique<WLEDController>(ip));
        } else {
            LogInfo(VB_PLUGIN, "controller type not found '%d'\n", type);
        }
        return nullptr;
    }

    void SetControllerTestModeOn(std::string const& ip, std::string const& type, bool checkMulti) {
        if(ip.find(",") != std::string::npos) {
            auto ips = split(ip, ',');
            for(auto const& ip_ : ips) {
                if(checkMulti && !IsInMultiSyncList(ip_)) {
                    continue;
                }
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type);
                if(controllerItem)controllerItem->setTestModeOn();
            }
        } else {
            if(checkMulti && !IsInMultiSyncList(ip)) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type);
            if(controllerItem)controllerItem->setTestModeOn();
        }
    }

    void SetControllerTestModeOff(std::string const& ip, std::string const& type, bool checkMulti) {
        if(ip.find(",") != std::string::npos) {
            auto ips = split(ip, ',');
            for(auto const& ip_ : ips) {
                if(checkMulti && !IsInMultiSyncList(ip_)) {
                    continue;
                }
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type);
                if(controllerItem)controllerItem->setTestModeOff();
            }
        } else {
            if(checkMulti && !IsInMultiSyncList(ip)) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type);
            if(controllerItem)controllerItem->setTestModeOff();
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
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type);
                if(controllerItem)controllerItem->setTestModeOn();
            }
        } else {
            auto type = GetMultiSyncType(ip);
            if(type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type);
            if(controllerItem)controllerItem->setTestModeOn();
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
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type);
                if(controllerItem)controllerItem->setTestModeOff();
            }
        } else {
            auto type = GetMultiSyncType(ip);
            if(type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type);
            if(controllerItem)controllerItem->setTestModeOff();
        }
    }

    void ToggleControllerTestMode(std::string const& ip, std::string const& type, bool checkMulti) {
        if(ip.find(",") != std::string::npos) {
            auto ips = split(ip, ',');
            for(auto const& ip_ : ips) {
                if(checkMulti && !IsInMultiSyncList(ip_)) {
                    continue;
                }
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type);
                if(controllerItem)controllerItem->toggleTestMode();
            }
        } else {
            if(checkMulti && !IsInMultiSyncList(ip)) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type);
            if(controllerItem)controllerItem->toggleTestMode();
        }
    }

    void ToggleControllerTestMode(std::string const& ip) {
        if(ip.find(",") != std::string::npos) {
            auto ips = split(ip, ',');
            for(auto const& ip_ : ips) {
                auto type = GetMultiSyncType(ip);
            if(type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
                std::unique_ptr<ControllerBase> controllerItem = MakeController(ip_, type);
                if(controllerItem)controllerItem->toggleTestMode();
            }
        } else {
            auto type = GetMultiSyncType(ip);
            if(type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(ip, type);
            if(controllerItem)controllerItem->toggleTestMode();
        }
    }

    void SetAllControllerTestModeOn() {        
        for (auto system : multiSync->GetLocalSystems()){
            if(system.type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(system.address, system.type);
            if(controllerItem)controllerItem->setTestModeOn();
        }
        for (auto system : multiSync->GetRemoteSystems()){
            if(system.type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(system.address, system.type);
            if(controllerItem)controllerItem->setTestModeOn();
        }
    }

    void SetAllControllerTestModeOff() {
        for (auto system : multiSync->GetLocalSystems()){
            if(system.type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(system.address, system.type);
            if(controllerItem)controllerItem->setTestModeOff();
        }
        for (auto system : multiSync->GetRemoteSystems()){
            if(system.type == MultiSyncSystemType::kSysTypeUnknown) {
                return;
            }
            std::unique_ptr<ControllerBase> controllerItem = MakeController(system.address, system.type);
            if(controllerItem)controllerItem->setTestModeOff();
        }
    }

    bool IsInMultiSyncList(std::string const& ip)
    {
        for (auto system : multiSync->GetLocalSystems()){
            if(system.address == ip){
                return true;
            }
        }
        for (auto system : multiSync->GetRemoteSystems()){
            if(system.address == ip){
                 return true;
            }
        }
        LogInfo(VB_PLUGIN, "IP Address not found in Multisync local list '%s'\n", ip.c_str());
        return false;
    }

    MultiSyncSystemType GetMultiSyncType(std::string const& ip)
    {
        for (auto system : multiSync->GetLocalSystems()){
            if(system.address == ip){
                return system.type;
            }
        }
        for (auto system : multiSync->GetRemoteSystems()){
            if(system.address == ip){
                return system.type;
            }
        }
        LogInfo(VB_PLUGIN, "IP Address not found in Multisync local list '%s'\n", ip.c_str());
        return MultiSyncSystemType::kSysTypeUnknown;
    }
};

extern "C" {
    FPPPlugin *createPlugin() {
        return new ControllerPlugin();
    }
}
