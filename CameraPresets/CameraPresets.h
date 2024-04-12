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
 void onUnload() override;
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
    struct CP_ImportedCode {
        CP_CameraSettings camera_settings;
        bool is_open;
    };
    bool HideMovementButtons = false;
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
    void DeletePlayerFromFile(std::string playername);
    void SaveToFile(std::string data);
    void GetAllCodes(std::string inputcode);
    std::string CreateSettingString(CP_CameraSettings camera);
    std::vector<CP_CameraSettings> GetProPreset(std::string substring);
    std::vector<CP_CameraSettings> ProPlayerCameras;
    CP_CameraSettings parseCode(const std::string& input);
    void DumpSave(std::string data);
    void LoadSave();
    std::vector<CP_ImportedCode> ImportedCodes;
    std::vector<CP_CameraSettings> cameras;
    CP_CameraSettings PlayerCameraSettings;
    CP_CameraSettings tempCamera;
    std::string InputCode;
    std::string SearchName;
    std::string bind = "F3";
    std::string CodeAdder;
    std::string PresetName;
    std::string CameraFolder = (gameWrapper->GetDataFolder() / "cameras_rlcs.data").string();
public:
    void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
    void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
