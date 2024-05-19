#pragma once

#include "GuiBase.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"
#include <fstream>
#include <regex>
#include <iostream>
#include <Windows.h>
#include "version.h"
#include <filesystem>

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
    struct CP_Version {
        int vn;
        std::string vd;
    };
    bool HideMovementButtons = false;
    bool CreatePreset = false;
    bool NoCodeError = false;
    bool CopyCodeMessage = false;
    bool RenameModal = false;
    bool ImportCode = false;
    bool DeleteWindow = false;
    bool settingsChanged = false;
    int InputNameError = 0;
    int selected = 0;
    int oldSelected = 0;
    std::string ProPlayerSearch;
    std::string FreestylePlayerSearch;
    std::string CCPlayerSearch;
    bool isValidCode(std::string code);
    void DeletePlayerFromFile(std::string playername, std::filesystem::path path);
    void SaveToFile(std::string data, std::filesystem::path path);
    void GetAllCodes(std::string inputcode);
    void FRenamePlayer(std::string playername, std::string nPlayername);
    std::string CreateSettingString(CP_CameraSettings camera);
    std::vector<CP_CameraSettings> GetProPreset(std::string substring, const char* file);
    std::vector<CP_CameraSettings> ProPlayerCameras;
    std::vector<CP_CameraSettings> FreestylePlayerCameras;
    std::vector<CP_CameraSettings> CCPlayerCameras;
    CP_CameraSettings parseCode(const std::string& input);
    void DumpSave(std::string data);
    void LoadSave();
    std::vector<CP_ImportedCode> ImportedCodes;
    std::vector<CP_CameraSettings> cameras;
    CP_CameraSettings PlayerCameraSettings;
    CP_CameraSettings tempCamera;
    std::string InputCode;
    ImGuiStyle CameraPresetsStyle;
    std::string RenameBuffer;
    std::string SearchName;

    std::string CodeAdder;
    std::string PresetName;
    std::string CameraFolder = (gameWrapper->GetDataFolder() / "cameras_rlcs.data").string();
    
    ImVec4 accent = ImVec4(9 / 255.f, 204.f / 255.f, 162.f / 255.f, 0.70f);
    ImVec4 hover = ImVec4{ 17.f / 255.f, 167.f / 255.f, 140.f / 255.f, 1.0f };
    ImVec4 primary = ImVec4(2.f / 255.f, 190.f / 255.f, 190.f / 255.f, 0.50f);

    ImGuiStyle backup;
public:
    void RenderSettings() override; // Uncomment if you wanna render your own tab in the settings menu
    void RenderWindow() override; // Uncomment if you want to render your own plugin window
};
