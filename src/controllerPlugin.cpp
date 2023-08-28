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

#include "genius.h"
#include "falconV4.h"
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
        ControllerTestOnCommand(ControllerPlugin *p) : Command("Set Test Mode On"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address"));
            args.push_back(CommandArg("type", "string", "Controller Type").setDefaultValue("FalconV4"));
            args.push_back(CommandArg("ports", "int", "Set Number of Ports").setRange(0, 128).setDefaultValue("16"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";
            std::string type = "FalconV4";
            int outputs = 16;
            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            if (args.size() >= 2) {
                type = args[1];
            }
            if (args.size() >= 3) {
                outputs = std::stoi(args[2]);
            }
            plugin->SetControllerTestModeOn(ipAddress, type, outputs);
            return std::make_unique<Command::Result>("Test Mode On");
        }
        ControllerPlugin *plugin;
    };      

    class ControllerTestOffCommand : public Command {
    public:
        ControllerTestOffCommand(ControllerPlugin *p) : Command("Set Test Mode Off"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address")); 
            args.push_back(CommandArg("type", "string", "Controller Type").setDefaultValue("FalconV4"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";
            std::string type = "FalconV4";
            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            if (args.size() >= 2) {
                type = args[1];
            }
            plugin->SetControllerTestModeOff(ipAddress, type);
            return std::make_unique<Command::Result>("Test Mode Off");
        }
        ControllerPlugin *plugin;
    };

    void registerCommand() {
        CommandManager::INSTANCE.addCommand(new ControllerTestOnCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerTestOffCommand(this));
    }

    void SetControllerTestModeOn(std::string const& ip, std::string const& type, int outputs) {
        //TPLinkSwitch tplinkSwitch(ip, 1, plug_num);
        //if(state){
        //    tplinkSwitch.setRelayOn();
        //} else{
        //    tplinkSwitch.setRelayOff();
        //}
    }

    void SetControllerTestModeOff(std::string const& ip, std::string const& type) {
        //TPLinkLight tplinkLight(ip, 1);
       // tplinkLight.setLightOff();
    }
};


extern "C" {
    FPPPlugin *createPlugin() {
        return new ControllerPlugin();
    }
}
