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
    ControllerPluginPlugin() : FPPPlugin("fpp-plugin-ControllerPlugin") {
        LogInfo(VB_PLUGIN, "Initializing Controller Plugin\n");
        readFiles();
        registerCommand();
    }
    virtual ~TPLinkPlugin() {

    }

    class ControllerTestOnCommand : public Command {
    public:
        ControllerTestOnCommand(TPLinkPlugin *p) : Command("Set Test Mode On"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address"));
            args.push_back(CommandArg("type", "string", "Controller Type").setDefaultValue("FalconV4"));
            args.push_back(CommandArg("ports", "int", "Set Number of Ports").setRange(0, 128).setDefaultValue("16"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";
            bool bulbOn = true;
            int plug_num = 0;
            if (args.size() >= 1) {
                ipAddress = args[0];
            }
            if (args.size() >= 2) {
                bulbOn = args[1]=="true";
            }
            if (args.size() >= 3) {
                plug_num = std::stoi(args[2]);
            }
            plugin->SetSwitchState(ipAddress, bulbOn, plug_num);
            return std::make_unique<Command::Result>("Test Mode On");
        }
        TPLinkPlugin *plugin;
    };      

    class ControllerTestOffCommand : public Command {
    public:
        ControllerTestOffCommand(TPLinkPlugin *p) : Command("Set Test Mode Off"), plugin(p) {
            args.push_back(CommandArg("IP", "string", "IP Address")); 
            args.push_back(CommandArg("type", "string", "Controller Type").setDefaultValue("FalconV4"));
        }
        
        virtual std::unique_ptr<Command::Result> run(const std::vector<std::string> &args) override {
            std::string ipAddress = "";
            if (args.size() >= 1) {
                ipAddress = args[0];
            }

            plugin->SetLightOff(ipAddress);
            return std::make_unique<Command::Result>("Test Mode Off");
        }
        TPLinkPlugin *plugin;
    };

    void registerCommand() {
        CommandManager::INSTANCE.addCommand(new ControllerTestOnCommand(this));
        CommandManager::INSTANCE.addCommand(new ControllerTestOffCommand(this));
    }

    void SetSwitchState(std::string const& ip, bool state, int plug_num) {
        //TPLinkSwitch tplinkSwitch(ip, 1, plug_num);
        //if(state){
        //    tplinkSwitch.setRelayOn();
        //} else{
        //    tplinkSwitch.setRelayOff();
        //}
    }

    void SetLightOff(std::string const& ip) {
        //TPLinkLight tplinkLight(ip, 1);
       // tplinkLight.setLightOff();
    }
};


extern "C" {
    FPPPlugin *createPlugin() {
        return new TPLinkPlugin();
    }
}
