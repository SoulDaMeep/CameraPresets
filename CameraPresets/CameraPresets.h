#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include <fstream>
#include <regex>
#include <Windows.h>
#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class CameraPresets: public BakkesMod::Plugin::BakkesModPlugin
 ,public SettingsWindowBase // Uncomment if you wanna render your own tab in the settings menu
 ,public PluginWindowBase // Uncomment if you want to render your own plugin window
{

 //std::shared_ptr<bool> enabled;

 //Boilerplate
 void onLoad() override;

 std::vector<std::string> Bundles = {
 "Jessie 110 100 -3 0.5 2.00 280.0 7.00\nSquishy 110 90 -5 0.5 1.00 270.0 6.00\nTorment 110 100 -5 0.4 1.00 270.0 8.00\nGimmick 110 100 -3 0.5 1.00 270.0 10.00\nFlakes 107 100 -5 1.0 1.00 310.0 7.00\nHockser 110 90 -4 0.4 1.00 270.0 5.00\nRollDizz 110 100 -4 0.7 1.00 240.0 7.00\nayjacks 110 110 -3 0.6 1.00 270.0 4.00\nMemory 110 110 -3 0.4 1.00 270.0 6.00\nAllushin 110 90 -3 0.4 1.00 260.0 3.00\nAtomic 110 90 -4 0.4 1.00 270.0 5.00\nGarrettG 110 100 -5 0.4 1.00 270.0 5.00\njstn 110 90 -4 0.4 1.00 270.0 5.00\nTurbopolsa 110 110 -5 0.4 1.00 270.0 6.00\nExplosiveGyro 110 100 -5 0.4 1.00 270.0 4.00\nretals 109 90 -4 0.4 1.00 270.0 5.00\nmist 109 100 -4 0.4 1.00 270.0 5.00\nWonder 110 90 -6 0.4 1.00 280.0 3.00\nAyyJayy 110 100 -5 0.4 1.00 260.0 7.00\nKronovi 110 100 -4 0.6 1.00 280.0 3.00\nSypical 110 100 -5 0.4 1.00 270.0 5.00\nAxB 110 100 -4 0.4 1.00 270.0 5.00\nArsenal 110 100 -5 0.4 1.00 270.0 10.00\nShock 110 110 -3 0.4 1.00 260.0 2.00\nSatthew 110 100 -5 0.4 1.00 260.0 4.00\nDappur 110 100 -4 0.5 1.00 270.0 5.00\nRapid 110 100 -3 0.7 1.00 240.0 6.00\nSeaBass 110 110 -4 0.6 1.00 260.0 5.00\nAeon 110 100 -5 0.3 1.00 270.0 5.00\nForky 110 100 -3 0.4 1.00 270.0 5.00\nRizzo 108 110 -4 0.2 1.00 280.0 4.00\nJKnaps 108 90 -5 0.4 2.00 260.0 3.00\nChicago 110 100 -3 0.5 1.00 270.0 8.00\nDeevo 109 100 -5 0.4 2.00 250.0 6.00\nBluey 110 100 -3 0.4 1.00 260.0 7.00\nRonaky 110 100 -3 0.5 1.00 270.0 3.00\nViolentPanda 110 110 -3 0.6 1.00 270.0 3.00\nYukeo 110 110 -3 1.0 2.00 300.0 2.00\nAztral 110 90 -4 0.4 1.00 270.0 7.00\nKuxir97 110 90 -6 1.0 1.00 260.0 4.00\nSpeed 109 110 -3 0.7 1.00 260.0 5.00\nal0t 110 90 -4 0.7 1.00 280.0 7.00\nFerra 110 100 -5 0.6 1.00 270.0 5.00\nChausette45 110 110 -3 0.4 1.00 280.0 5.00\nfruity 110 110 -3 0.6 1.00 270.0 4.00\nFairyPeak 110 110 -3 0.6 1.00 270.0 4.00\nScrub 110 100 -4 0.7 1.00 270.0 7.00\nKaydop 110 110 -3 0.7 1.00 270.0 3.00\nFlamE 110 90 -3 0.8 1.00 230.0 3.00\nFreaKii 110 110 -3 0.4 1.00 270.0 10.00\nKassio 110 100 -5 0.6 1.00 270.0 7.00\nTigreee 110 90 -3 0.4 1.00 270.0 7.00\nAlpha54 110 110 -3 0.3 1.00 270.0 4.00\nremkoe 110 100 -4 0.4 1.00 270.0 6.00\nMetsanauris 110 90 -4 1.0 1.00 260.0 6.00\nTorsos 110 100 -2 0.6 1.00 250.0 10.00\noKhaliD 110 100 -3 0.5 1.00 270.0 3.00\n",
 "zen 110 100 -3 0.35 1.00 270 4.00\nVatira 110 100 -4 0.35 1.5 260 4.70\nAlpha54 110 110 -3 0.35 1.00 270 3.70\nRise 110 90 -5 0.40 1.00 260 6.50\nM0nkeyM00n 109 110 -3 0.6 1.10 260 4.00\nRadosin 110 100 -3 0.70 1.40 240 4.20\nBeastMode 110 90 -4 0.45 1.20 270 7.00\nAtow 110 100 -3 0.4 1.3 270 4.2\nLJ 110 100 -5 0.55 1.30 270 5.00\nExoTiiK 110 110 -3 0.5 1.00 270 4.00",
 "Jstn 110 100 -3 0.35 1.30 270 4.70\nGarrettG 110 100 -3 0.35 1.20 270 4.20\nSquishyMuffinz 110 90 -3 0.35 1.20 270 7.00\nTurbopolsa 110 100 -4 0.4 1.50 270 4.40\nKaydop 110 100 -3 0.45 1.00 270 3.60\nScrubKilla 110 100 -4 0.4 1.3 270 6.80\nChicago 110 90 -3 0.45 1.40 270 5.30\nFairyPeak 110 100 -3 0.35 1.40 270 4.70\nKronovi 110 100 -3 0.45 1.40 260 1.40",
 "PulseFire 110 100 -3 0.6 1.2 270 4.20\nPulseJxsh 110 90 -5 0.15 1.70 260 7.50\nPulseMaktuf 110 90 -6 0.00 3.5 240 3.00\nPulseMK 110 100 -4 0.4 1.20 270 5.00\nPulseJZR 110 90 -2 0.30 1.60 250 4.00"
 };
 //FOV HEIGHT ANGLE STIFFNESS TRANSITIONSPEED DISTANCE SWIVELSPEED
    struct CP_CameraSettings {
        std::string name;
        int FOV;
        int Distance;
        int Height;
        int Angle;
        float Stiffness;
        float SwivelSpeed;
        float TransitionSpeed;
        std::string code;
    };
    bool CreatePreset = false;
    bool NoCodeError = false;
    bool CopyCodeMessage = false;
    bool ImportCode = false;
    bool DeleteWindow = false;
    bool settingsChanged = false;
    bool InputNameError = false;
    int selected = 0;
    int oldSelected = 0;
    std::string ProPlayerSearch;
    bool isValidCode(std::string code);
    void SaveDataToFile(std::string playername, std::vector<std::string> values);
    void DeletePlayerFromFile(std::string playername);
    void SaveToFile(std::string data);
    void GetAllCodes(std::string inputcode);
    std::string CreateSettingString(CP_CameraSettings camera);
    std::vector<CP_CameraSettings> GetProPreset(std::string substring);
    std::vector<CP_CameraSettings> ProPlayerCameras;
    CP_CameraSettings parseCode(const std::string& input);
    std::vector<CP_CameraSettings> ImportedCodes;
    std::vector<CP_CameraSettings> cameras;
    CP_CameraSettings PlayerCameraSettings;
    CP_CameraSettings tempCamera;
    std::string InputCode;
    std::string SearchName;
    std::string bind = "F5";
    std::string CodeAdder;
    std::string PresetName;
    std::string CameraFolder = (gameWrapper->GetDataFolder() / "cameras_rlcs.data").string();

public:
    void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
    void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
